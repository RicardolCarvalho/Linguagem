#!/usr/bin/env python3
# peixe_vm_with_vars.py - Adaptador com suporte à declaração automática de variáveis

import sys
import os
import re

# Diretório onde está o código principal da linguagem
DIR = os.path.dirname(os.path.abspath(__file__))
PYTHON_DIR = os.path.join(DIR, "python")
sys.path.append(DIR)
sys.path.append(PYTHON_DIR)

# Importar módulos do interpretador Python
try:
    from python.node import SymbolTable, Node, IntVal, BoolVal, StrVal, Identifier
    from python.node import Assignment, Print, BinOp, Block
except ImportError as e:
    print(f"Erro ao importar módulos Python: {e}")
    sys.exit(1)

# Classe personalizada que implementa uma VM simples para a linguagem peixe
class PeixeVM:
    def __init__(self):
        self.symbol_table = {}  # Tabela de símbolos simplificada
    
    def set_variable(self, name, value):
        """Define o valor de uma variável"""
        self.symbol_table[name] = value
    
    def get_variable(self, name):
        """Obtém o valor de uma variável"""
        if name not in self.symbol_table:
            raise ValueError(f"Variável não definida: {name}")
        return self.symbol_table[name]
    
    def evaluate_expression(self, expr):
        """Avalia uma expressão"""
        # Se for um número literal
        if isinstance(expr, int) or (isinstance(expr, str) and expr.isdigit()):
            return int(expr)
        
        # Se for uma variável
        elif isinstance(expr, str) and not (expr == 'salmao' or expr == 'tilapia'):
            # Verificar se é uma lista (rede[...])
            if expr.startswith('rede[') and expr.endswith(']'):
                # Extrair conteúdo dentro dos colchetes
                list_content = expr[5:-1].strip()
                items = []
                
                # Se a lista não estiver vazia
                if list_content:
                    # Dividir por vírgulas
                    for item_str in list_content.split(','):
                        item_str = item_str.strip()
                        try:
                            # Converter para inteiro ou avaliar expressão
                            if item_str.isdigit():
                                item = int(item_str)
                            else:
                                item = self.evaluate_expression(item_str)
                            items.append(item)
                        except ValueError:
                            raise ValueError(f"Item de lista não é válido: {item_str}")
                
                return items
            # Verificar se é uma string literal
            elif expr.startswith('"') and expr.endswith('"'):
                return expr[1:-1]  # Remover as aspas
            # Senão, é uma variável
            else:
                return self.get_variable(expr)
        
        # Se for um booleano
        elif expr == 'salmao':
            return True
        elif expr == 'tilapia':
            return False
        
        # Outros tipos de expressões podem ser adicionados aqui
        else:
            # Verificar operadores de comparação e aritméticos
            for op in [' > ', ' < ', ' == ', ' + ', ' - ', ' * ', ' / ']:
                if op in expr:
                    left, right = expr.split(op, 1)
                    left_val = self.evaluate_expression(left.strip())
                    right_val = self.evaluate_expression(right.strip())
                    
                    if op == ' > ':
                        return left_val > right_val
                    elif op == ' < ':
                        return left_val < right_val
                    elif op == ' == ':
                        return left_val == right_val
                    elif op == ' + ':
                        return left_val + right_val
                    elif op == ' - ':
                        return left_val - right_val
                    elif op == ' * ':
                        return left_val * right_val
                    elif op == ' / ':
                        return left_val // right_val  # Divisão inteira para seguir o padrão C
            
            raise ValueError(f"Expressão não suportada: {expr}")
    
    def execute(self, code):
        """Executa o código peixe"""
        # Dividir o código em linhas e remover comentários
        lines = []
        for line in code.split('\n'):
            if '//' in line:
                line = line[:line.index('//')]
            line = line.strip()
            if line:
                lines.append(line)
        
        # Processar o código linha por linha
        i = 0
        while i < len(lines):
            line = lines[i]
            
            # Atribuição: nome = valor;
            if '=' in line and line.endswith(';'):
                parts = line.split('=', 1)
                var_name = parts[0].strip()
                value_str = parts[1].strip().rstrip(';').strip()
                
                # Avaliar o valor
                value = self.evaluate_expression(value_str)
                
                # Definir a variável
                self.set_variable(var_name, value)
                print(f"Atribuição: {var_name} = {value}")
                i += 1
            
            # Fale (print): fale(expr);
            elif line.startswith('fale(') and line.endswith(');'):
                expr_str = line[5:-2].strip()
                
                # Avaliar a expressão
                value = self.evaluate_expression(expr_str)
                
                # Imprimir o valor
                if isinstance(value, list):
                    # Formatar listas
                    print(f"Saída: [{', '.join(map(str, value))}]")
                else:
                    print(f"Saída: {value}")
                i += 1
            
            # Estrutura condicional: se (condição) { ... } senão { ... }
            elif line.startswith('se (') and ')' in line:
                # Extrair a condição
                cond_start = line.index('(') + 1
                cond_end = line.index(')')
                cond_str = line[cond_start:cond_end].strip()
                
                # Avaliar a condição
                condition = self.evaluate_expression(cond_str)
                
                # Encontrar o bloco 'então'
                then_block, else_block = self.extract_blocks(lines, i)
                
                # Executar o bloco apropriado
                if condition:
                    # Executar o bloco 'então'
                    then_code = '\n'.join(then_block)
                    self.execute(then_code)
                    i += len(then_block) + 2  # +2 para 'se (cond) {' e '}'
                else:
                    # Verificar se há um bloco 'senão'
                    if else_block:
                        else_code = '\n'.join(else_block)
                        self.execute(else_code)
                        i += len(then_block) + len(else_block) + 4  # +4 para 'se (cond) {', '}', 'senão {', '}'
                    else:
                        i += len(then_block) + 2  # +2 para 'se (cond) {' e '}'
            
            # Loop while: enquanto (condição) { ... }
            elif line.startswith('enquanto (') and ')' in line:
                # Extrair a condição
                cond_start = line.index('(') + 1
                cond_end = line.index(')')
                cond_str = line[cond_start:cond_end].strip()
                
                # Encontrar o bloco do loop
                loop_block, _ = self.extract_blocks(lines, i)
                loop_code = '\n'.join(loop_block)
                
                # Executar o loop enquanto a condição for verdadeira
                while self.evaluate_expression(cond_str):
                    self.execute(loop_code)
                
                i += len(loop_block) + 2  # +2 para 'enquanto (cond) {' e '}'
            
            # Outros tipos de instruções podem ser adicionados aqui
            else:
                raise ValueError(f"Instrução não suportada: {line}")
        
        print("Programa concluído com sucesso!")
        return 0
    
    def extract_blocks(self, lines, start_index):
        """Extrai blocos de código entre { } para estruturas condicionais e loops"""
        then_block = []
        else_block = []
        
        # Encontrar o início do bloco 'então'
        i = start_index
        while i < len(lines) and '{' not in lines[i]:
            i += 1
        
        if i >= len(lines):
            raise ValueError("Bloco de código não encontrado")
        
        # Contar as chaves para encontrar o final do bloco 'então'
        brace_count = 1
        i += 1  # Pular a linha com a chave de abertura
        
        while i < len(lines) and brace_count > 0:
            if '{' in lines[i]:
                brace_count += 1
            if '}' in lines[i]:
                brace_count -= 1
            
            if brace_count > 0:
                then_block.append(lines[i])
            i += 1
        
        # Verificar se há um bloco 'senão'
        if i < len(lines) and 'senão' in lines[i]:
            # Encontrar o início do bloco 'senão'
            while i < len(lines) and '{' not in lines[i]:
                i += 1
            
            if i >= len(lines):
                raise ValueError("Bloco 'senão' não encontrado")
            
            # Contar as chaves para encontrar o final do bloco 'senão'
            brace_count = 1
            i += 1  # Pular a linha com a chave de abertura
            
            while i < len(lines) and brace_count > 0:
                if '{' in lines[i]:
                    brace_count += 1
                if '}' in lines[i]:
                    brace_count -= 1
                
                if brace_count > 0:
                    else_block.append(lines[i])
                i += 1
        
        return then_block, else_block

def run_peixe_file(peixe_file):
    """Executa um arquivo peixe"""
    try:
        # Ler o arquivo
        with open(peixe_file, 'r') as f:
            code = f.read()
        
        print(f"Executando {peixe_file}...")
        print("-" * 40)
        
        # Criar a VM e executar o código
        vm = PeixeVM()
        result = vm.execute(code)
        
        print("-" * 40)
        return result
    
    except Exception as e:
        print(f"Erro: {e}")
        return 1

def main():
    """Função principal"""
    if len(sys.argv) != 2:
        print(f"Uso: {sys.argv[0]} <arquivo.pesca>")
        return 1
    
    peixe_file = sys.argv[1]
    
    if not os.path.exists(peixe_file):
        print(f"Erro: Arquivo não encontrado: {peixe_file}")
        return 1
    
    return run_peixe_file(peixe_file)

if __name__ == "__main__":
    sys.exit(main())
