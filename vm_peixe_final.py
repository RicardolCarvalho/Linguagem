#!/usr/bin/env python3
# vm_peixe_final.py - Implementação simplificada da VM para a linguagem peixe

import sys
import os
import re

class PeixeVM:
    def __init__(self):
        self.variables = {}  # Tabela de símbolos
    
    def execute_file(self, filename):
        """Executa um arquivo da linguagem peixe"""
        try:
            # Ler o arquivo
            with open(filename, 'r') as file:
                code = file.read()
            
            print(f"Executando {filename}...")
            print("-" * 50)
            
            # Remover comentários e preparar o código
            code_lines = []
            for line in code.split('\n'):
                if '//' in line:
                    line = line[:line.index('//')]
                line = line.strip()
                
                # Separar a linha "} senão {" em duas partes
                if line == "} senão {":
                    code_lines.append("}")
                    code_lines.append("senão {")
                elif line:
                    code_lines.append(line)
            
            # Executar o código linha por linha
            i = 0
            while i < len(code_lines):
                i = self.execute_statement(code_lines, i)
            
            print("-" * 50)
            print("Programa executado com sucesso!")
            return 0
            
        except Exception as e:
            print(f"Erro: {e}")
            import traceback
            traceback.print_exc()
            return 1
    
    def execute_statement(self, lines, i):
        """Executa uma instrução e retorna o próximo índice"""
        line = lines[i].strip()
        
        # Atribuição: var = valor;
        if '=' in line and line.endswith(';') and not line.startswith('se') and not line.startswith('enquanto'):
            var_name, expr = line.split('=', 1)
            var_name = var_name.strip()
            expr = expr.strip()[:-1]  # Remover o ponto e vírgula
            value = self.evaluate_expression(expr)
            self.variables[var_name] = value
            print(f"Atribuição: {var_name} = {value}")
            return i + 1
        
        # Comando print: fale(expr);
        elif line.startswith('fale(') and line.endswith(');'):
            expr = line[5:-2].strip()
            value = self.evaluate_expression(expr)
            if isinstance(value, list):
                print(f"Saída: [{', '.join(map(str, value))}]")
            else:
                print(f"Saída: {value}")
            return i + 1
        
        # Estrutura condicional: se (condição) { ... } senão { ... }
        elif line.startswith('se ('):
            # Extrair a condição
            cond_start = line.index('(') + 1
            cond_end = line.index(')')
            condition = line[cond_start:cond_end].strip()
            result = self.evaluate_expression(condition)
            
            # Encontrar o bloco 'então'
            then_start, then_end = self.find_block(lines, i)
            
            # Verificar se há um bloco 'senão'
            else_start, else_end = -1, -1
            has_else = False
            
            if then_end + 1 < len(lines):
                senao_line = lines[then_end + 1].strip()
                if senao_line == "} senão {" or senao_line.startswith("senão"):
                    # Se a estrutura for "} senão {" ou começa com "senão"
                    if senao_line == "} senão {":
                        # A linha já contém as chaves, ajustar para encontrar o bloco
                        lines[then_end + 1] = "senão {"
                    
                    else_start, else_end = self.find_block(lines, then_end + 1)
                    has_else = True
            
            # Executar o bloco apropriado
            if result:
                print(f"Condição '{condition}' é verdadeira")
                # Executar o bloco 'então'
                j = then_start + 1  # Pular a linha com a chave de abertura
                while j < then_end:
                    j = self.execute_statement(lines, j)
                
                # Pular o bloco 'senão' se existir
                if has_else:
                    return else_end + 1
                else:
                    return then_end + 1
            else:
                print(f"Condição '{condition}' é falsa")
                # Executar o bloco 'senão' se existir
                if has_else:
                    j = else_start + 1  # Pular a linha com a chave de abertura
                    while j < else_end:
                        j = self.execute_statement(lines, j)
                    return else_end + 1
                else:
                    return then_end + 1
        
        # Loop: enquanto (condição) { ... }
        elif line.startswith('enquanto ('):
            # Extrair a condição
            cond_start = line.index('(') + 1
            cond_end = line.index(')')
            condition = line[cond_start:cond_end].strip()
            
            # Encontrar o bloco do loop
            loop_start, loop_end = self.find_block(lines, i)
            
            # Executar o loop enquanto a condição for verdadeira
            iteration = 0
            while self.evaluate_expression(condition):
                iteration += 1
                print(f"Loop iteração {iteration}")
                
                # Executar o corpo do loop
                j = loop_start + 1  # Pular a linha com a chave de abertura
                while j < loop_end:
                    j = self.execute_statement(lines, j)
                
                # Verificar se atingimos o limite de iterações (para evitar loops infinitos)
                if iteration >= 100:
                    print("Aviso: Limite de iterações atingido. Possível loop infinito.")
                    break
            
            return loop_end + 1
        
        # Chave de fechamento ou abertura soltas
        elif line == '{' or line == '}':
            return i + 1
        
        # Comando não reconhecido
        else:
            print(f"Aviso: Comando não reconhecido: {line}")
            return i + 1
    
    def find_block(self, lines, start_index):
        """Encontra um bloco delimitado por chaves"""
        # Pular para a linha com a chave de abertura
        i = start_index
        while i < len(lines) and '{' not in lines[i]:
            i += 1
        
        if i >= len(lines):
            raise ValueError(f"Não foi possível encontrar o início do bloco a partir da linha {start_index}")
        
        block_start = i
        
        # Encontrar a chave de fechamento correspondente
        brace_count = 1
        i += 1
        
        while i < len(lines) and brace_count > 0:
            if '{' in lines[i]:
                brace_count += 1
            if '}' in lines[i]:
                brace_count -= 1
            i += 1
        
        if brace_count > 0:
            raise ValueError(f"Não foi possível encontrar o fim do bloco que começa na linha {block_start}")
        
        block_end = i - 1
        
        return block_start, block_end
    
    def evaluate_expression(self, expr):
        """Avalia uma expressão da linguagem peixe"""
        expr = expr.strip()
        
        # Número literal
        if expr.isdigit():
            return int(expr)
        
        # String literal
        if expr.startswith('"') and expr.endswith('"'):
            return expr[1:-1]  # Remover as aspas
        
        # Booleanos
        if expr == 'salmao':
            return True
        if expr == 'tilapia':
            return False
        
        # Lista (rede[...])
        if expr.startswith('rede[') and expr.endswith(']'):
            items_str = expr[5:-1].strip()
            items = []
            
            if items_str:
                for item in items_str.split(','):
                    items.append(self.evaluate_expression(item.strip()))
            
            return items
        
        # Operadores de comparação
        if ' > ' in expr:
            left, right = expr.split(' > ', 1)
            return self.evaluate_expression(left) > self.evaluate_expression(right)
        if ' < ' in expr:
            left, right = expr.split(' < ', 1)
            return self.evaluate_expression(left) < self.evaluate_expression(right)
        if ' == ' in expr:
            left, right = expr.split(' == ', 1)
            return self.evaluate_expression(left) == self.evaluate_expression(right)
        
        # Operadores aritméticos
        if ' + ' in expr:
            left, right = expr.split(' + ', 1)
            return self.evaluate_expression(left) + self.evaluate_expression(right)
        if ' - ' in expr:
            left, right = expr.split(' - ', 1)
            return self.evaluate_expression(left) - self.evaluate_expression(right)
        if ' * ' in expr:
            left, right = expr.split(' * ', 1)
            return self.evaluate_expression(left) * self.evaluate_expression(right)
        if ' / ' in expr:
            left, right = expr.split(' / ', 1)
            return self.evaluate_expression(left) // self.evaluate_expression(right)
        
        # Variável
        if expr in self.variables:
            return self.variables[expr]
        
        # Normalizar operadores (adicionar espaços)
        for op in ['>', '<', '==', '+', '-', '*', '/']:
            if op in expr and f' {op} ' not in expr:
                expr = expr.replace(op, f' {op} ')
                return self.evaluate_expression(expr)
        
        raise ValueError(f"Expressão não reconhecida: {expr}")

def main():
    if len(sys.argv) != 2:
        print(f"Uso: {sys.argv[0]} <arquivo.pesca>")
        return 1
    
    filename = sys.argv[1]
    if not os.path.exists(filename):
        print(f"Erro: Arquivo não encontrado: {filename}")
        return 1
    
    vm = PeixeVM()
    return vm.execute_file(filename)

if __name__ == "__main__":
    sys.exit(main())
