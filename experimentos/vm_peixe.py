#!/usr/bin/env python3
# vm_peixe.py - Máquina Virtual para a linguagem Peixe

import sys
import json
import os

class PeixeVM:
    def __init__(self):
        self.memory = {}   # Memória para variáveis
        self.stack = []    # Pilha de execução
        self.program = []  # Instruções do programa
    
    def load_program(self, bytecode_file):
        """Carrega um programa de bytecode de um arquivo JSON"""
        try:
            with open(bytecode_file, 'r') as f:
                self.program = json.load(f)
            print(f"Programa carregado: {len(self.program)} instruções")
        except Exception as e:
            print(f"Erro ao carregar o programa: {e}")
            sys.exit(1)
    
    def execute(self):
        """Executa o programa carregado"""
        pc = 0  # Program Counter
        
        while pc < len(self.program):
            instr = self.program[pc]
            opcode = instr["op"]
            
            if opcode == "PUSH":
                # Empilha um valor
                self.stack.append(instr["value"])
            
            elif opcode == "POP":
                # Remove um valor da pilha
                if self.stack:
                    self.stack.pop()
            
            elif opcode == "STORE":
                # Armazena o valor do topo da pilha em uma variável
                if self.stack:
                    value = self.stack.pop()
                    self.memory[instr["var"]] = value
                    # print(f"Variável {instr['var']} = {value}")
            
            elif opcode == "LOAD":
                # Carrega o valor de uma variável para a pilha
                if instr["var"] in self.memory:
                    self.stack.append(self.memory[instr["var"]])
                else:
                    print(f"Erro: Variável '{instr['var']}' não definida")
                    sys.exit(1)
            
            elif opcode == "ADD":
                # Adiciona os dois valores do topo da pilha
                if len(self.stack) >= 2:
                    b = self.stack.pop()
                    a = self.stack.pop()
                    if isinstance(a, int) and isinstance(b, int):
                        self.stack.append(a + b)
                    else:
                        print(f"Erro: Não é possível adicionar {a} e {b}")
                        sys.exit(1)
            
            elif opcode == "SUB":
                # Subtrai os dois valores do topo da pilha
                if len(self.stack) >= 2:
                    b = self.stack.pop()
                    a = self.stack.pop()
                    if isinstance(a, int) and isinstance(b, int):
                        self.stack.append(a - b)
                    else:
                        print(f"Erro: Não é possível subtrair {a} e {b}")
                        sys.exit(1)
            
            elif opcode == "MUL":
                # Multiplica os dois valores do topo da pilha
                if len(self.stack) >= 2:
                    b = self.stack.pop()
                    a = self.stack.pop()
                    if isinstance(a, int) and isinstance(b, int):
                        self.stack.append(a * b)
                    else:
                        print(f"Erro: Não é possível multiplicar {a} e {b}")
                        sys.exit(1)
            
            elif opcode == "DIV":
                # Divide os dois valores do topo da pilha
                if len(self.stack) >= 2:
                    b = self.stack.pop()
                    a = self.stack.pop()
                    if isinstance(a, int) and isinstance(b, int):
                        if b == 0:
                            print("Erro: Divisão por zero")
                            sys.exit(1)
                        self.stack.append(a // b)  # Divisão inteira
                    else:
                        print(f"Erro: Não é possível dividir {a} e {b}")
                        sys.exit(1)
            
            elif opcode == "CMP":
                # Compara os dois valores do topo da pilha
                if len(self.stack) >= 2:
                    b = self.stack.pop()
                    a = self.stack.pop()
                    op = instr["cmp"]
                    
                    if op == "gt":
                        self.stack.append(1 if a > b else 0)
                    elif op == "lt":
                        self.stack.append(1 if a < b else 0)
                    elif op == "eq":
                        self.stack.append(1 if a == b else 0)
                    elif op == "ne":
                        self.stack.append(1 if a != b else 0)
                    elif op == "ge":
                        self.stack.append(1 if a >= b else 0)
                    elif op == "le":
                        self.stack.append(1 if a <= b else 0)
                    else:
                        print(f"Erro: Operador de comparação inválido: {op}")
                        sys.exit(1)
            
            elif opcode == "JMP":
                # Salto incondicional
                pc = instr["addr"] - 1  # -1 porque vamos incrementar pc no final
            
            elif opcode == "JZ":
                # Salto se o topo da pilha for zero
                if self.stack:
                    value = self.stack.pop()
                    if value == 0:
                        pc = instr["addr"] - 1  # -1 porque vamos incrementar pc no final
            
            elif opcode == "JNZ":
                # Salto se o topo da pilha não for zero
                if self.stack:
                    value = self.stack.pop()
                    if value != 0:
                        pc = instr["addr"] - 1  # -1 porque vamos incrementar pc no final
            
            elif opcode == "PRINT":
                # Imprime o valor do topo da pilha
                if self.stack:
                    value = self.stack.pop()
                    if isinstance(value, bool):
                        print("true" if value else "false")
                    elif isinstance(value, list):
                        print(f"[{', '.join(map(str, value))}]")
                    else:
                        print(value)
            
            elif opcode == "MAKE_LIST":
                # Cria uma lista com os elementos na pilha
                count = instr["count"]
                if len(self.stack) >= count:
                    items = []
                    for _ in range(count):
                        items.insert(0, self.stack.pop())
                    self.stack.append(items)
            
            elif opcode == "HALT":
                # Termina a execução
                break
            
            else:
                print(f"Erro: Instrução desconhecida: {opcode}")
                sys.exit(1)
            
            pc += 1  # Avança para a próxima instrução
        
        print("Programa executado com sucesso!")
        return 0

def main():
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <arquivo_bytecode.json>")
        sys.exit(1)
    
    vm = PeixeVM()
    vm.load_program(sys.argv[1])
    return vm.execute()

if __name__ == "__main__":
    sys.exit(main())
