class Token:
    def __init__(self, type, valor):
        self.type = type
        self.valor = valor

class Tokenizer:
    def __init__(self, source):
        self.source = source
        self.pos = 0
        self.next = None
        self.selectNext()

    def selectNext(self):
        s = self.source
        L = len(s)
        # pula espaços
        while self.pos < L and s[self.pos].isspace():
            self.pos += 1
        if self.pos >= L:
            self.next = Token('EOF','')
            return

        ch = s[self.pos]

        # string literal
        if ch == '"':
            self.pos += 1
            buf = ""
            while self.pos < L and s[self.pos] != '"':
                buf += s[self.pos]
                self.pos += 1
            if self.pos >= L:
                raise ValueError("Fim de string não encontrado")
            self.pos += 1
            self.next = Token('STRING', buf)
            return

        # número
        if ch.isdigit():
            num = ""
            while self.pos < L and s[self.pos].isdigit():
                num += s[self.pos]
                self.pos += 1
            self.next = Token('NUMBER', int(num))
            return

        # identificador / palavra‐chave
        if ch.isalpha() or ch == '_':
            word = ""
            while self.pos < L and (s[self.pos].isalnum() or s[self.pos]=='_'):
                word += s[self.pos]
                self.pos += 1
            # keywords da Linguagem de Pesca
            if word == "fale":
                self.next = Token('PRINT', word)
            elif word == "se":
                self.next = Token('IF', word)
            elif word in ("senão","senao"):
                self.next = Token('ELSE', word)
            elif word == "enquanto":
                self.next = Token('WHILE', word)
            elif word == "reader":
                self.next = Token('READER', word)
            elif word == "var":
                self.next = Token('VAR', word)
            elif word == "salmao":
                self.next = Token('BOOL', True)
            elif word == "tilapia":
                self.next = Token('BOOL', False)
            elif word == "rede":
                self.next = Token('REDE', word)
            else:
                self.next = Token('IDENTIFIER', word)
            return

        # operadores e símbolos
        two = s[self.pos:self.pos+2]
        if two == "++":
            self.next = Token('CONCAT','++'); self.pos+=2; return
        if two == "==":
            self.next = Token('EQUAL_EQUAL','=='); self.pos+=2; return
        if two == "&&":
            self.next = Token('AND','&&'); self.pos+=2; return
        if two == "||":
            self.next = Token('OR','||'); self.pos+=2; return

        # single‐char tokens
        single = {
            '+':'PLUS','-':'MINUS','*':'MULT','/':'DIV',
            '(': 'OPEN_PAR', ')':'CLOSE_PAR',
            '{':'OPEN_BLOCK','}':'CLOSE_BLOCK',
            '[':'OPEN_BRACK',']':'CLOSE_BRACK',
            ';':'SEMI',',':'COMMA',':':'COLON'
        }
        if ch in single:
            self.next = Token(single[ch], ch)
            self.pos += 1
            return

        # = (assign ou == já visto), < >
        if ch == '=':
            self.next = Token('EQUAL','=')
            self.pos += 1
            return
        if ch == '<':
            self.next = Token('LESS','<'); self.pos+=1; return
        if ch == '>':
            self.next = Token('GREATER','>'); self.pos+=1; return
        if ch == '!':
            self.next = Token('NOT','!'); self.pos+=1; return

        raise ValueError(f"Token inválido: '{ch}'")

class PrePro:
    @staticmethod
    def filter(code: str) -> str:
        # remove comentários /* */ e //...
        out = []
        i = 0
        L = len(code)
        comment = False
        while i < L:
            if not comment and code[i:i+2]=="/*":
                comment = True; i+=2; continue
            if comment and code[i:i+2]=="*/":
                comment=False; i+=2; continue
            if not comment:
                if code[i:i+2]=="//":
                    # pula até \n
                    i+=2
                    while i<L and code[i]!="\n": i+=1
                    continue
                out.append(code[i])
            i+=1
        return ''.join(out)