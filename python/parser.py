from tokenizer import Tokenizer, PrePro
from node import *

class Parser:
    def __init__(self, code):
        filtered = PrePro.filter(code)
        self.tok = Tokenizer(filtered)

    def eat(self, typ):
        if self.tok.next.type != typ:
            raise ValueError(f"Esperado {typ}, achado {self.tok.next.type}")
        val = self.tok.next.valor
        self.tok.selectNext()
        return val

    def parse(self):
        # programa obrigatoriamente entre { }
        stmts = []
        if self.tok.next.type != 'OPEN_BLOCK':
            raise ValueError("Programa deve começar com '{'")
        self.eat('OPEN_BLOCK')
        while self.tok.next.type != 'CLOSE_BLOCK':
            stmts.append(self.parseStatement())
        self.eat('CLOSE_BLOCK')
        if self.tok.next.type!='EOF':
            raise ValueError("Tokens sobrando no fim")
        return Block(stmts)

    def parseStatement(self):
        t = self.tok.next.type
        # var decl
        if t=='VAR':
            self.eat('VAR')
            name = self.eat('IDENTIFIER')
            self.eat('COLON')
            vtype = self.eat('IDENTIFIER')
            init = None
            if self.tok.next.type=='EQUAL':
                self.eat('EQUAL')
                init = self.parseBoolExpression()
            self.eat('SEMI')
            return VarDecl(name, vtype, init)

        # assignment
        if t=='IDENTIFIER':
            name = self.eat('IDENTIFIER')
            self.eat('EQUAL')
            expr = self.parseBoolExpression()
            self.eat('SEMI')
            return Assignment(name, expr)

        # print
        if t=='PRINT':
            self.eat('PRINT')
            self.eat('OPEN_PAR')
            expr = self.parseBoolExpression()
            self.eat('CLOSE_PAR')
            self.eat('SEMI')
            return Print(expr)

        # if
        if t=='IF':
            self.eat('IF')
            self.eat('OPEN_PAR')
            cond = self.parseBoolExpression()
            self.eat('CLOSE_PAR')
            then_blk = self.parseBlock()
            else_blk = None
            if self.tok.next.type=='ELSE':
                self.eat('ELSE')
                else_blk = self.parseBlock()
            return IfOP(cond, then_blk, else_blk)

        # while
        if t=='WHILE':
            self.eat('WHILE')
            self.eat('OPEN_PAR')
            cond = self.parseBoolExpression()
            self.eat('CLOSE_PAR')
            body = self.parseBlock()
            return WhileOP(cond, body)

        # bloco solto não permitido
        raise ValueError("Comando inválido: "+t)

    def parseBlock(self):
        self.eat('OPEN_BLOCK')
        stmts=[]
        while self.tok.next.type!='CLOSE_BLOCK':
            stmts.append(self.parseStatement())
        self.eat('CLOSE_BLOCK')
        return Block(stmts)

    def parseBoolExpression(self):
        node = self.parseBoolTerm()
        while self.tok.next.type=='OR':
            self.eat('OR')
            node = BinOp('OR',[node, self.parseBoolTerm()])
        return node

    def parseBoolTerm(self):
        node = self.parseBoolFactor()
        while self.tok.next.type=='AND':
            self.eat('AND')
            node = BinOp('AND',[node, self.parseBoolFactor()])
        return node

    def parseBoolFactor(self):
        node = self.parseExpression()
        if self.tok.next.type in ('EQUAL_EQUAL','LESS','GREATER'):
            op = self.tok.next.type
            self.eat(op)
            right = self.parseExpression()
            return BinOp(op,[node,right])
        return node

    def parseExpression(self):
        node = self.parseTerm()
        while self.tok.next.type in ('PLUS','MINUS','CONCAT'):
            op = self.tok.next.type
            self.eat(op)
            node = BinOp(op,[node,self.parseTerm()])
        return node

    def parseTerm(self):
        node = self.parseFactor()
        while self.tok.next.type in ('MULT','DIV'):
            op=self.tok.next.type
            self.eat(op)
            node=BinOp(op,[node,self.parseFactor()])
        return node

    def parseFactor(self):
        t = self.tok.next
        if t.type=='NUMBER':
            self.eat('NUMBER'); return IntVal(t.valor)
        if t.type=='STRING':
            self.eat('STRING'); return StrVal(t.valor)
        if t.type=='BOOL':
            self.eat('BOOL');   return BoolVal(t.valor)
        if t.type=='IDENTIFIER':
            self.eat('IDENTIFIER'); return Identifier(t.valor)
        if t.type in ('PLUS','MINUS','NOT'):
            op = t.type; self.eat(op)
            child = self.parseFactor()
            return UnOp(op,[child])
        if t.type=='READER':
            self.eat('READER')
            self.eat('OPEN_PAR'); self.eat('CLOSE_PAR')
            return Read()
        if t.type=='OPEN_PAR':
            self.eat('OPEN_PAR')
            node = self.parseBoolExpression()
            self.eat('CLOSE_PAR')
            return node
        raise ValueError("Esperado fator, achado "+t.type)
