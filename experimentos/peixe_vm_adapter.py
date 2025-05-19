#!/usr/bin/env python3
# peixe_vm_adapter.py - Adaptador para executar a linguagem peixe usando o interpretador Python

import sys
import os
import re

# Diretório onde está o código principal da linguagem
DIR = os.path.dirname(os.path.abspath(__file__))
PYTHON_DIR = os.path.join(DIR, "python")
sys.path.append(DIR)
sys.path.append(PYTHON_DIR)

# Importar os módulos necessários do interpretador Python
from python.tokenizer import Tokenizer, PrePro
from python.parser import Parser
from python.node import SymbolTable

def run_with_python_vm(peixe_file):
    """Executa um arquivo da linguagem peixe usando o interpretador Python"""
    
    try:
        # Ler o arquivo peixe
        with open(peixe_file, 'r') as f:
            peixe_code = f.read()
        
        # Adicionar chaves ({ }) ao redor do código para que o parser aceite
        code = "{\n" + peixe_code + "\n}"
        
        # Executar o código usando o interpretador Python
        print(f"Executando {peixe_file} com a VM Python...")
        
        # Analisar e executar
        tree = Parser(code).parse()
        st = SymbolTable()
        tree.evaluate(st)
        
        print(f"\nPrograma executado com sucesso!")
        return 0
        
    except Exception as e:
        print(f"Erro durante a execução: {e}")
        return 1

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
