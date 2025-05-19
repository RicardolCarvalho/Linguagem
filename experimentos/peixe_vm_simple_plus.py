#!/usr/bin/env python3
# peixe_vm_simple_plus.py - VM simples para a linguagem peixe com suporte a condicionais e loops

import sys
import os
import re

class PeixeVM:
    def __init__(self):
        self.variables = {}
    
    def set_variable(self, name, value):
        self.variables[name] = value
        return value
    
    def get_variable(self, name):
        if name not in self.variables:
            raise ValueError(f"Variável não definida: {name}")
        return self.variables[name]
    
    def evaluate_expression(self, expr):
        """Avalia uma expressão simples"""
        expr = expr.strip()
        
        # Verificar se é um número literal
        if expr.isdigit():
            return int(expr)
        
        # Verificar se é um booleano
        if expr == 'salmao':
            return True
        if expr == 'tilapia':
            return False
        
        # Verificar se é uma string literal
        if expr.startswith('"') and expr.endswith('"'):
            return expr[1:-1]  # Remover as aspas
        
        # Verificar se é uma lista
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
            return self.evaluate_expression(left) // self.evaluate_expression(right)  # Divisão inteira
        
        # Se não for nenhum dos casos acima, deve ser uma variável
        return self.get_variable(expr)
    
    def run_code(self, code):
        """Executa o código da linguagem peixe"""
        lines = self.preprocess_code(code)
        i = 0
        
        while i < len(lines):
            line = lines[i].strip()
            
            # Pular linhas vazias
            if not line:
                i += 1
                continue
            
            # Atribuição: var = valor;
            if '=' in line and line.endswith(';') and not line.startswith('se') and not line.startswith('enquanto'):
                var_name, expr = line.split('=', 1)
                var_name = var_name.strip()
                expr = expr.strip()[:-1]  # Remover o ponto e vírgula
                value = self.evaluate_expression(expr)
                self.set_variable(var_name, value)
                print(f"Atribuição: {var_name} = {value}")
                i += 1
                continue
            
            # Comando fale: fale(expr);
            if line.startswith('fale(') and line.endswith(');'):
                expr = line[5:-2].strip()
                value = self.evaluate_expression(expr)
                
                # Formatação especial para listas
                if isinstance(value, list):
                    print(f"Saída: [{', '.join(map(str, value))}]")
                else:
                    print(f"Saída: {value}")
                i += 1
                continue
            
            # Estrutura condicional: se (condição) {
            if line.startswith('se (') and line.endswith('{'):
                # Extrair a condição
                cond_start = line.index('(') + 1
                cond_end = line.index(')')
                condition = line[cond_start:cond_end].strip()
                result = self.evaluate_expression(condition)
                
                # Encontrar o bloco 'então' entre { e }
                then_block = []
                i += 1  # Passar para a próxima linha após 'se (condição) {'
                brace_level = 1
                
                while i < len(lines) and brace_level > 0:
                    if lines[i].endswith('{'):
                        brace_level += 1
                    elif lines[i] == '}':
                        brace_level -= 1
                        if brace_level == 0:
                            break
                    
                    if brace_level > 0:
                        then_block.append(lines[i])
                    i += 1
                
                # Verificar se há um bloco 'senão'
                else_block = []
                has_else = False
                
                if i+1 < len(lines) and lines[i+1] == 'senão {':
                    has_else = True
                    i += 2  # Pular 'senão {'
                    brace_level = 1
                    
                    while i < len(lines) and brace_level > 0:
                        if lines[i].endswith('{'):
                            brace_level += 1
                        elif lines[i] == '}':
                            brace_level -= 1
                            if brace_level == 0:
                                break
                        
                        if brace_level > 0:
                            else_block.append(lines[i])
                        i += 1
                
                # Executar o bloco apropriado
                if result:
                    # Executar o bloco 'então'
                    then_code = '\n'.join(then_block)
                    self.run_code(then_code)
                elif has_else:
                    # Executar o bloco 'senão'
                    else_code = '\n'.join(else_block)
                    self.run_code(else_code)
                
                # Avançar para a próxima instrução após o if/else
                i += 1
                continue
            
            # Loop: enquanto (condição) {
            if line.startswith('enquanto (') and line.endswith('{'):
                # Extrair a condição
                cond_start = line.index('(') + 1
                cond_end = line.index(')')
                condition = line[cond_start:cond_end].strip()
                
                # Encontrar o corpo do loop
                loop_body = []
                i += 1  # Passar para a próxima linha após 'enquanto (condição) {'
                brace_level = 1
                
                while i < len(lines) and brace_level > 0:
                    if lines[i].endswith('{'):
                        brace_level += 1
                    elif lines[i] == '}':
                        brace_level -= 1
                        if brace_level == 0:
                            break
                    
                    if brace_level > 0:
                        loop_body.append(lines[i])
                    i += 1
                
                # Executar o loop enquanto a condição for verdadeira
                while self.evaluate_expression(condition):
                    loop_code = '\n'.join(loop_body)
                    self.run_code(loop_code)
                
                # Avançar para a próxima instrução após o loop
                i += 1
                continue
            
            # Se chegou aqui, não reconheceu o comando
            print(f"Aviso: Comando não reconhecido: {line}")
            i += 1
    
    def preprocess_code(self, code):
        """Preprocessa o código removendo comentários e tratando chaves corretamente"""
        # Remover comentários
        code_without_comments = ''
        i = 0
        while i < len(code):
            if i < len(code) - 1 and code[i:i+2] == '//':
                # Pular até o final da linha
                while i < len(code) and code[i] != '\n':
                    i += 1
                if i < len(code):
                    code_without_comments += '\n'
            else:
                code_without_comments += code[i]
            i += 1
        
        # Adicionar espaços ao redor dos operadores para facilitar o parsing
        for op in ['>', '<', '==', '+', '-', '*', '/']:
            code_without_comments = code_without_comments.replace(op, f' {op} ')
        
        # Dividir em linhas e remover linhas vazias
        raw_lines = [line.strip() for line in code_without_comments.split('\n')]
        lines = [line for line in raw_lines if line]
        
        # Normalizar o código para facilitar o processamento
        processed_lines = []
        for line in lines:
            # Garantir que cada linha tenha no máximo uma instrução
            if ';' in line and not (line.startswith('enquanto') or line.startswith('se')):
                instructions = line.split(';')
                for instr in instructions:
                    instr = instr.strip()
                    if instr:
                        processed_lines.append(instr + ';')
                continue
            
            # Tratar chaves de abertura
            if '{' in line:
                parts = line.split('{')
                processed_lines.append(parts[0].strip() + ' {')
                continue
            
            # Tratar chaves de fechamento
            if '}' in line:
                if 'senão' in line:
                    processed_lines.append('}')
                    processed_lines.append('senão {')
                else:
                    processed_lines.append('}')
                continue
            
            # Linha normal
            processed_lines.append(line)
        
        return processed_lines

def main():
    if len(sys.argv) != 2:
        print(f"Uso: {sys.argv[0]} <arquivo.pesca>")
        return 1
    
    filename = sys.argv[1]
    if not os.path.exists(filename):
        print(f"Erro: Arquivo não encontrado: {filename}")
        return 1
    
    try:
        with open(filename, 'r') as f:
            code = f.read()
        
        print(f"Executando {filename}...")
        print("-" * 50)
        
        vm = PeixeVM()
        vm.run_code(code)
        
        print("-" * 50)
        print("Programa concluído com sucesso!")
        return 0
    
    except Exception as e:
        print(f"Erro: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == "__main__":
    sys.exit(main())
