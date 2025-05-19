#!/usr/bin/env python3
# vm_peixe_simple.py - Máquina Virtual simplificada para a linguagem Peixe

import sys
import json

# Dicionário de variáveis
vars = {}

def execute_bytecode(code):
    print("Executando programa...")
    for instr in code:
        if instr["op"] == "ASSIGN":
            var_name = instr["var"]
            value = instr["value"]
            vars[var_name] = value
            print(f"Atribuição: {var_name} = {value}")
        
        elif instr["op"] == "PRINT":
            var_name = instr["var"]
            if var_name in vars:
                value = vars[var_name]
                if isinstance(value, list):
                    # Formatar listas com colchetes
                    print(f"Saída: [{', '.join(map(str, value))}]")
                else:
                    print(f"Saída: {value}")
            else:
                print(f"Erro: Variável '{var_name}' não definida")
                return 1

    print("Programa concluído com sucesso!")
    return 0

def main():
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <arquivo_bytecode.json>")
        return 1
    
    bytecode_file = sys.argv[1]
    try:
        with open(bytecode_file, 'r') as f:
            bytecode = json.load(f)
            print(f"Bytecode carregado: {len(bytecode)} instruções")
    except Exception as e:
        print(f"Erro ao carregar bytecode: {e}")
        return 1
    
    return execute_bytecode(bytecode)

if __name__ == "__main__":
    sys.exit(main())
