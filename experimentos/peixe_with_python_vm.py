#!/usr/bin/env python3
# peixe_with_python_vm.py - Adaptador para executar a linguagem peixe usando o interpretador Python

import sys
import os
import re

# Diretório onde estão os módulos Python
PYTHON_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "python")
sys.path.append(PYTHON_DIR)

def convert_to_ast_format(peixe_code):
    """Converte código da linguagem peixe para o formato que o interpretador Python entende"""
    
    # Remover comentários e espaços em branco extras
    lines = []
    for line in peixe_code.split('\n'):
        # Remover comentários de linha
        if '//' in line:
            line = line[:line.index('//')]
        
        line = line.strip()
        if line:
            lines.append(line)
    
    # Converter para o formato esperado pelo interpretador Python
    ast_code = "{\n"
    
    for line in lines:
        # Substituir "salmao" por "true" e "tilapia" por "false"
        line = re.sub(r'\bsalmao\b', 'true', line)
        line = re.sub(r'\btilapia\b', 'false', line)
        
        # Substituir "se" por "if", "senão" por "else", "enquanto" por "while", "fale" por "print"
        line = re.sub(r'\bse\b', 'if', line)
        line = re.sub(r'\bsenão\b', 'else', line)
        line = re.sub(r'\benquanto\b', 'while', line)
        line = re.sub(r'\bfale\b', 'print', line)
        
        # Substituir "rede" por "list"
        line = re.sub(r'\brede\b', 'list', line)
        
        ast_code += "  " + line + "\n"
    
    ast_code += "}"
    return ast_code

def run_with_python_vm(peixe_file, temp_file=None):
    """Executa um arquivo da linguagem peixe usando o interpretador Python"""
    
    # Ler o arquivo peixe
    with open(peixe_file, 'r') as f:
        peixe_code = f.read()
    
    # Converter para o formato do interpretador Python
    ast_code = convert_to_ast_format(peixe_code)
    
    # Arquivo temporário para o código convertido
    if temp_file is None:
        temp_file = os.path.splitext(peixe_file)[0] + ".ast.tmp"
    
    # Escrever o código convertido no arquivo temporário
    with open(temp_file, 'w') as f:
        f.write(ast_code)
    
    try:
        # Importar o módulo main do interpretador Python
        from python.main import Parser, SymbolTable
        
        # Executar o código usando o interpretador Python
        print(f"Executando {peixe_file} com a VM Python...")
        code = open(temp_file, encoding='utf8').read()
        
        try:
            tree = Parser(code).parse()
            st = SymbolTable()
            tree.evaluate(st)
            print(f"\nPrograma executado com sucesso!")
            return 0
        except Exception as e:
            print(f"Erro durante a execução: {e}")
            return 1
    
    finally:
        # Remover o arquivo temporário
        if os.path.exists(temp_file):
            os.remove(temp_file)

def main():
    """Ponto de entrada principal"""
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <arquivo.pesca>")
        return 1
    
    peixe_file = sys.argv[1]
    
    if not os.path.exists(peixe_file):
        print(f"Erro: Arquivo não encontrado: {peixe_file}")
        return 1
    
    return run_with_python_vm(peixe_file)

if __name__ == "__main__":
    sys.exit(main())
