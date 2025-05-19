#!/usr/bin/env python3
# compile_peixe.py - Compilador simples para gerar bytecode da linguagem Peixe

import sys
import os
import json
import re

# Definição de padrões regex para tokenização
PATTERNS = [
    ('WHITESPACE', r'[ \t\r\n]+'),
    ('COMMENT', r'//.*'),
    ('NUMBER', r'\d+'),
    ('STRING', r'"([^"\\]|\\.)*"'),
    ('BOOL_TRUE', r'salmao'),
    ('BOOL_FALSE', r'tilapia'),
    ('IF', r'se'),
    ('ELSE', r'senão'),
    ('WHILE', r'enquanto'),
    ('PRINT', r'fale'),
    ('LIST', r'rede'),
    ('IDENTIFIER', r'[a-zA-Z_][a-zA-Z0-9_]*'),
    ('EQUALS', r'='),
    ('PLUS', r'\+'),
    ('MINUS', r'-'),
    ('MULTIPLY', r'\*'),
    ('DIVIDE', r'/'),
    ('GREATER', r'>'),
    ('LESS', r'<'),
    ('GREATER_EQUAL', r'>='),
    ('LESS_EQUAL', r'<='),
    ('EQUAL_EQUAL', r'=='),
    ('NOT_EQUAL', r'!='),
    ('SEMICOLON', r';'),
    ('COMMA', r','),
    ('LEFT_PAREN', r'\('),
    ('RIGHT_PAREN', r'\)'),
    ('LEFT_BRACE', r'{'),
    ('RIGHT_BRACE', r'}'),
    ('LEFT_BRACKET', r'\['),
    ('RIGHT_BRACKET', r'\]'),
]

class Token:
    def __init__(self, type, value, line, column):
        self.type = type
        self.value = value
        self.line = line
        self.column = column
    
    def __str__(self):
        return f"Token({self.type}, {repr(self.value)}, {self.line}:{self.column})"

class Lexer:
    def __init__(self, source):
        self.source = source
        self.tokens = []
        self.line = 1
        self.column = 1
    
    def tokenize(self):
        remaining = self.source
        
        while remaining:
            match = None
            
            for token_type, pattern in PATTERNS:
                regex = re.compile('^' + pattern)
                match = regex.search(remaining)
                
                if match:
                    value = match.group(0)
                    
                    if token_type not in ['WHITESPACE', 'COMMENT']:
                        if token_type == 'STRING':
                            # Remover as aspas e processar escape sequences
                            string_content = value[1:-1]
                            # Aqui você pode processar escape sequences se necessário
                            token = Token(token_type, string_content, self.line, self.column)
                        else:
                            token = Token(token_type, value, self.line, self.column)
                        self.tokens.append(token)
                    
                    # Atualizar linha e coluna
                    newlines = value.count('\n')
                    if newlines > 0:
                        self.line += newlines
                        self.column = len(value) - value.rindex('\n')
                    else:
                        self.column += len(value)
                    
                    remaining = remaining[len(value):]
                    break
            
            if not match:
                print(f"Erro léxico na linha {self.line}, coluna {self.column}: Caractere inválido")
                print(f"Contexto: {remaining[:20]}...")
                sys.exit(1)
        
        self.tokens.append(Token('EOF', '', self.line, self.column))
        return self.tokens

class Compiler:
    def __init__(self, tokens):
        self.tokens = tokens
        self.current = 0
        self.instructions = []
        self.labels = {}
        self.next_label = 0
        self.loops = []  # Pilha para guardar labels de loops (inicio, fim)
    
    def create_label(self):
        """Cria um novo rótulo"""
        label = f"L{self.next_label}"
        self.next_label += 1
        return label
    
    def set_label(self, label):
        """Define um rótulo no endereço atual"""
        self.labels[label] = len(self.instructions)
    
    def emit(self, instruction):
        """Adiciona uma instrução ao bytecode"""
        self.instructions.append(instruction)
    
    def resolve_jumps(self):
        """Resolve os saltos no bytecode"""
        for i, instr in enumerate(self.instructions):
            if "label" in instr:
                label = instr["label"]
                if label in self.labels:
                    self.instructions[i]["addr"] = self.labels[label]
                else:
                    print(f"Erro: Rótulo não definido: {label}")
                    sys.exit(1)
                del self.instructions[i]["label"]
    
    def peek(self):
        """Retorna o token atual sem avançar"""
        return self.tokens[self.current]
    
    def advance(self):
        """Avança para o próximo token"""
        if not self.is_at_end():
            self.current += 1
        return self.previous()
    
    def previous(self):
        """Retorna o token anterior"""
        return self.tokens[self.current - 1]
    
    def is_at_end(self):
        """Verifica se chegou ao fim dos tokens"""
        return self.peek().type == 'EOF'
    
    def check(self, token_type):
        """Verifica se o token atual é do tipo especificado"""
        if self.is_at_end():
            return False
        return self.peek().type == token_type
    
    def match(self, *token_types):
        """Verifica se o token atual corresponde a algum dos tipos especificados"""
        for token_type in token_types:
            if self.check(token_type):
                self.advance()
                return True
        return False
    
    def consume(self, token_type, message):
        """Consome um token do tipo especificado ou gera um erro"""
        if self.check(token_type):
            return self.advance()
        
        token = self.peek()
        print(f"Erro na linha {token.line}, coluna {token.column}: {message}")
        print(f"Token encontrado: {token.type} ('{token.value}')")
        sys.exit(1)
    
    def compile(self):
        """Inicia a compilação do programa"""
        self.program()
        self.resolve_jumps()
        return self.instructions
    
    def program(self):
        """Compila o programa"""
        while not self.is_at_end():
            self.statement()
    
    def statement(self):
        """Compila uma instrução"""
        if self.match('IF'):
            self.if_statement()
        elif self.match('WHILE'):
            self.while_statement()
        elif self.match('PRINT'):
            self.print_statement()
        elif self.match('IDENTIFIER'):
            self.assignment()
        else:
            token = self.peek()
            print(f"Erro na linha {token.line}, coluna {token.column}: Instrução inválida")
            print(f"Token encontrado: {token.type} ('{token.value}')")
            sys.exit(1)
    
    def if_statement(self):
        """Compila uma instrução if"""
        self.consume('LEFT_PAREN', "Esperado '(' após 'se'")
        self.expression()  # Condição
        self.consume('RIGHT_PAREN', "Esperado ')' após condição")
        
        else_label = self.create_label()
        end_label = self.create_label()
        
        # Salta para o else se a condição for falsa
        self.emit({"op": "JZ", "label": else_label})
        
        self.consume('LEFT_BRACE', "Esperado '{' após condição")
        self.block()  # Bloco then
        self.consume('RIGHT_BRACE', "Esperado '}' após bloco")
        
        # Salta para o fim após executar o bloco then
        self.emit({"op": "JMP", "label": end_label})
        
        # Marca o início do bloco else
        self.set_label(else_label)
        
        if self.match('ELSE'):
            self.consume('LEFT_BRACE', "Esperado '{' após 'senão'")
            self.block()  # Bloco else
            self.consume('RIGHT_BRACE', "Esperado '}' após bloco")
        
        # Marca o fim do if
        self.set_label(end_label)
    
    def while_statement(self):
        """Compila uma instrução while"""
        self.consume('LEFT_PAREN', "Esperado '(' após 'enquanto'")
        
        start_label = self.create_label()
        end_label = self.create_label()
        
        # Marca o início do loop
        self.set_label(start_label)
        
        # Guarda os rótulos para uso em instruções de controle
        self.loops.append((start_label, end_label))
        
        self.expression()  # Condição
        self.consume('RIGHT_PAREN', "Esperado ')' após condição")
        
        # Salta para o fim se a condição for falsa
        self.emit({"op": "JZ", "label": end_label})
        
        self.consume('LEFT_BRACE', "Esperado '{' após condição")
        self.block()  # Corpo do loop
        self.consume('RIGHT_BRACE', "Esperado '}' após bloco")
        
        # Volta para o início do loop
        self.emit({"op": "JMP", "label": start_label})
        
        # Marca o fim do loop
        self.set_label(end_label)
        
        # Remove os rótulos do loop atual
        self.loops.pop()
    
    def print_statement(self):
        """Compila uma instrução de impressão"""
        self.consume('LEFT_PAREN', "Esperado '(' após 'fale'")
        self.expression()  # Expressão a ser impressa
        self.consume('RIGHT_PAREN', "Esperado ')' após expressão")
        self.consume('SEMICOLON', "Esperado ';' após instrução")
        
        # Gera instrução para imprimir
        self.emit({"op": "PRINT"})
    
    def assignment(self):
        """Compila uma atribuição"""
        var_name = self.previous().value
        
        self.consume('EQUALS', f"Esperado '=' após variável '{var_name}'")
        self.expression()  # Expressão de valor
        self.consume('SEMICOLON', "Esperado ';' após expressão")
        
        # Gera instrução para armazenar
        self.emit({"op": "STORE", "var": var_name})
    
    def block(self):
        """Compila um bloco de código"""
        while not self.check('RIGHT_BRACE') and not self.is_at_end():
            self.statement()
    
    def expression(self):
        """Compila uma expressão"""
        self.comparison()
    
    def comparison(self):
        """Compila uma expressão de comparação"""
        self.addition()
        
        while True:
            if self.match('GREATER'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "gt"})
            elif self.match('LESS'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "lt"})
            elif self.match('GREATER_EQUAL'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "ge"})
            elif self.match('LESS_EQUAL'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "le"})
            elif self.match('EQUAL_EQUAL'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "eq"})
            elif self.match('NOT_EQUAL'):
                self.addition()
                self.emit({"op": "CMP", "cmp": "ne"})
            else:
                break
    
    def addition(self):
        """Compila uma expressão de adição ou subtração"""
        self.multiplication()
        
        while True:
            if self.match('PLUS'):
                self.multiplication()
                self.emit({"op": "ADD"})
            elif self.match('MINUS'):
                self.multiplication()
                self.emit({"op": "SUB"})
            else:
                break
    
    def multiplication(self):
        """Compila uma expressão de multiplicação ou divisão"""
        self.primary()
        
        while True:
            if self.match('MULTIPLY'):
                self.primary()
                self.emit({"op": "MUL"})
            elif self.match('DIVIDE'):
                self.primary()
                self.emit({"op": "DIV"})
            else:
                break
    
    def primary(self):
        """Compila uma expressão primária"""
        if self.match('NUMBER'):
            # Constante numérica
            value = int(self.previous().value)
            self.emit({"op": "PUSH", "value": value})
        
        elif self.match('STRING'):
            # Constante string
            value = self.previous().value
            self.emit({"op": "PUSH", "value": value})
        
        elif self.match('BOOL_TRUE'):
            # Constante booleana true
            self.emit({"op": "PUSH", "value": 1})
        
        elif self.match('BOOL_FALSE'):
            # Constante booleana false
            self.emit({"op": "PUSH", "value": 0})
        
        elif self.match('IDENTIFIER'):
            # Referência a variável
            var_name = self.previous().value
            self.emit({"op": "LOAD", "var": var_name})
        
        elif self.match('LEFT_PAREN'):
            # Expressão entre parênteses
            self.expression()
            self.consume('RIGHT_PAREN', "Esperado ')' após expressão")
        
        elif self.match('LIST'):
            # Lista
            self.consume('LEFT_BRACKET', "Esperado '[' após 'rede'")
            
            # Contar itens na lista
            count = 0
            
            if not self.check('RIGHT_BRACKET'):
                # Primeiro item
                self.expression()
                count += 1
                
                # Itens adicionais
                while self.match('COMMA'):
                    self.expression()
                    count += 1
            
            self.consume('RIGHT_BRACKET', "Esperado ']' após itens da lista")
            
            # Gera instrução para criar lista
            self.emit({"op": "MAKE_LIST", "count": count})
        
        else:
            token = self.peek()
            print(f"Erro na linha {token.line}, coluna {token.column}: Expressão inválida")
            print(f"Token encontrado: {token.type} ('{token.value}')")
            sys.exit(1)

def main():
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <arquivo_fonte.pesca> [saida.json]")
        sys.exit(1)
    
    # Define o nome do arquivo de saída
    input_file = sys.argv[1]
    if len(sys.argv) > 2:
        output_file = sys.argv[2]
    else:
        output_file = os.path.splitext(input_file)[0] + ".bytecode.json"
    
    print(f"Compilando {input_file} para {output_file}")
    
    # Lê o arquivo fonte
    try:
        with open(input_file, 'r') as f:
            source = f.read()
            print(f"Arquivo fonte lido: {len(source)} bytes")
    except Exception as e:
        print(f"Erro ao ler o arquivo fonte: {e}")
        sys.exit(1)
    
    # Tokeniza o código fonte
    lexer = Lexer(source)
    tokens = lexer.tokenize()
    print(f"Tokens gerados: {len(tokens)}")
    
    # Compila os tokens
    compiler = Compiler(tokens)
    bytecode = compiler.compile()
    print(f"Instruções geradas: {len(bytecode)}")
    
    # Adiciona instrução de parada no final
    bytecode.append({"op": "HALT"})
    
    # Salva o bytecode em um arquivo JSON
    try:
        with open(output_file, 'w') as f:
            json.dump(bytecode, f, indent=2)
        print(f"Bytecode gerado com sucesso: {output_file}")
    except Exception as e:
        print(f"Erro ao salvar o bytecode: {e}")
        sys.exit(1)
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
