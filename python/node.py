class SymbolTable:
    def __init__(self):
        self.table = {}
    def declare(self, name, typ, val):
        if name in self.table:
            raise ValueError(f"Variável já declarada: {name}")
        self.table[name] = (typ, val)
    def getter(self, name):
        if name not in self.table:
            raise ValueError(f"Variável não definida: {name}")
        return self.table[name]
    def setter(self, name, tup):
        if name not in self.table:
            raise ValueError(f"Variável não definida: {name}")
        self.table[name] = tup

class Node:
    def evaluate(self, st: SymbolTable):
        raise NotImplementedError()

class IntVal(Node):
    def __init__(self, v): self.value = v
    def evaluate(self, st): return (self.value, "i32")

class BoolVal(Node):
    def __init__(self, v): self.value = v
    def evaluate(self, st): return (self.value, "bool")

class StrVal(Node):
    def __init__(self, v): self.value = v
    def evaluate(self, st): return (self.value, "str")

class Identifier(Node):
    def __init__(self, name): self.value = name
    def evaluate(self, st):
        typ, val = st.getter(self.value)
        return (val, typ)

class NoOp(Node):
    def evaluate(self, st): return (None, None)

class VarDecl(Node):
    def __init__(self, name, typ, init):
        self.name, self.typ, self.init = name, typ, init
    def evaluate(self, st):
        if self.init:
            v, t = self.init.evaluate(st)
            if t != self.typ:
                raise ValueError(f"Tipo errado na init de {self.name}")
        else:
            v = 0 if self.typ=="i32" else False if self.typ=="bool" else ""
        st.declare(self.name, self.typ, v)
        return (v, self.typ)

class Assignment(Node):
    def __init__(self, name, expr):
        self.name, self.expr = name, expr
    def evaluate(self, st):
        v, t = self.expr.evaluate(st)
        decl_t, _ = st.getter(self.name)
        if decl_t != t:
            raise ValueError(f"Atribuição incompatível em {self.name}")
        st.setter(self.name, (t, v))
        return (v, t)

class Print(Node):
    def __init__(self, expr): self.expr=expr
    def evaluate(self, st):
        v, t = self.expr.evaluate(st)
        if t=="bool": print(str(v).lower())
        else:        print(v)
        return (None, None)

class Read(Node):
    def evaluate(self, st):
        v = int(input())
        return (v, "i32")

class BinOp(Node):
    def __init__(self, op, children):
        self.op, self.children = op, children
    def evaluate(self, st):
        l_val, l_t = self.children[0].evaluate(st)
        r_val, r_t = self.children[1].evaluate(st)
        # só exemplo com + - * / ++ && || == < >
        if self.op=="PLUS":
            if l_t!="i32" or r_t!="i32": raise
            return (l_val+r_val, "i32")
        if self.op=="CONCAT":
            return (str(l_val)+str(r_val), "str")
        if self.op=="AND":
            return (l_val and r_val, "bool")
        if self.op=="EQUAL_EQUAL":
            return (l_val==r_val, "bool")
        if self.op=="LESS":
            return (l_val<r_val, "bool")
        # … implemente os outros…
        raise ValueError("Operador não suportado:"+self.op)

class UnOp(Node):
    def __init__(self, op, children):
        self.op, self.children = op, children
    def evaluate(self, st):
        v, t = self.children[0].evaluate(st)
        if self.op=="MINUS" and t=="i32": return (-v,"i32")
        if self.op=="NOT"   and t=="bool":return (not v,"bool")
        raise

class Block(Node):
    def __init__(self, stmts): self.stmts=stmts
    def evaluate(self, st):
        result=(None,None)
        for s in self.stmts:
            result = s.evaluate(st)
        return result

class IfOP(Node):
    def __init__(self, cond, then_blk, else_blk=None):
        self.cond, self.then_blk, self.else_blk = cond, then_blk, else_blk
    def evaluate(self, st):
        v, t = self.cond.evaluate(st)
        if t!="bool": raise
        if v:    return self.then_blk.evaluate(st)
        elif self.else_blk:
                return self.else_blk.evaluate(st)
        return (None,None)

class WhileOP(Node):
    def __init__(self, cond, body):
        self.cond, self.body = cond, body
    def evaluate(self, st):
        while True:
            v, t = self.cond.evaluate(st)
            if t!="bool": raise
            if not v: break
            self.body.evaluate(st)
        return (None,None)
