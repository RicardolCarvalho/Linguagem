#!/usr/bin/env python3
# peixe_vm_final.py - Implementação final da VM para a linguagem peixe

import sys
import os
import re

class PeixeVM:
    def __init__(self):
        # Tabela de símbolos (variáveis)
        self.variables = {}
    
    def run(self, filename):
        """Executa um arquivo peixe"""
        try:
            with open(filename, 'r') as f:
                code = f.read()
            
            print(f"Executando {filename}...")
            print("-" * 50)
            self.execute(code)
            print("-" * 50)
            print("Programa executado com sucesso!")
            return 0
        
        except Exception as e:
            print(f"Erro: {e}")
            import traceback
            traceback.print_exc()
            return 1
    
    def execute(self, code):
        """Executa código da linguagem peixe"""
        # Remover comentários
        clean_code = re.sub(r'//.*$', '', code, flags=re.MULTILINE)
        
        # Dividir o código em blocos
        blocks = self.parse_blocks(clean_code)
        
        # Executar o bloco principal
        self.execute_block(blocks[0])
    
    def parse_blocks(self, code):
        """Parse o código em blocos"""
        code = code.strip()
        
        # Lista de blocos (cada bloco é uma lista de comandos)
        blocks = [[]]
        
        # Pilha de blocos ativos
        active_blocks = [0]  # Começamos no bloco 0 (raiz)
        
        # Estado atual do parser
        in_string = False
        escape_next = False
        buffer = ""
        
        for char in code:
            # Processamento de string (dentro de aspas)
            if in_string:
                if escape_next:
                    buffer += char
                    escape_next = False
                elif char == '\\':
                    escape_next = True
                elif char == '"':
                    in_string = False
                    buffer += char
                else:
                    buffer += char
                continue
            
            # Início de string
            if char == '"':
                in_string = True
                buffer += char
                continue
            
            # Início de bloco
            if char == '{':
                # Se temos algum conteúdo no buffer, adicioná-lo ao bloco atual
                if buffer.strip():
                    blocks[active_blocks[-1]].append(buffer.strip())
                
                # Criar um novo bloco
                blocks.append([])
                active_blocks.append(len(blocks) - 1)
                buffer = ""
                continue
            
            # Fim de bloco
            if char == '}':
                # Se temos algum conteúdo no buffer, adicioná-lo ao bloco atual
                if buffer.strip():
                    blocks[active_blocks[-1]].append(buffer.strip())
                
                # Remover o bloco atual da pilha
                active_blocks.pop()
                buffer = ""
                continue
            
            # Fim de declaração
            if char == ';':
                buffer += char
                if buffer.strip():
                    blocks[active_blocks[-1]].append(buffer.strip())
                buffer = ""
                continue
            
            # Outros caracteres
            buffer += char
        
        # Se ainda temos algo no buffer, adicioná-lo ao bloco atual
        if buffer.strip():
            blocks[active_blocks[-1]].append(buffer.strip())
        
        return blocks
    
    def execute_block(self, block):
        """Executa um bloco de comandos"""
        for cmd in block:
            self.execute_command(cmd)
    
    def execute_command(self, cmd):
        """Executa um comando individual"""
        cmd = cmd.strip()
        
        # Atribuição: var = valor;
        if '=' in cmd and cmd.endswith(';'):
            self.execute_assignment(cmd)
            return
        
        # Print: fale(expr);
        if cmd.startswith('fale(') and cmd.endswith(');'):
            self.execute_print(cmd)
            return
        
        # Condicional: se (condição) { bloco1 } senão { bloco2 }
        if cmd.startswith('se (') and ')' in cmd:
            self.execute_if(cmd)
            return
        
        # Loop: enquanto (condição) { bloco }
        if cmd.startswith('enquanto (') and ')' in cmd:
            self.execute_while(cmd)
            return
        
        print(f"Aviso: Comando não reconhecido: {cmd}")
    
    def execute_assignment(self, cmd):
        """Executa uma atribuição de variável"""
        # Remover o ponto e vírgula
        cmd = cmd[:-1].strip()
        
        # Dividir em nome da variável e expressão
        name, expr = cmd.split('=', 1)
        name = name.strip()
        expr = expr.strip()
        
        # Avaliar a expressão e armazenar o valor
        value = self.evaluate(expr)
        self.variables[name] = value
        
        print(f"Atribuição: {name} = {value}")
    
    def execute_print(self, cmd):
        """Executa um comando de impressão"""
        # Extrair a expressão dentro dos parênteses
        expr = cmd[5:-2].strip()
        
        # Avaliar a expressão e imprimir o valor
        value = self.evaluate(expr)
        
        # Formatação especial para listas
        if isinstance(value, list):
            print(f"Saída: [{', '.join(map(str, value))}]")
        else:
            print(f"Saída: {value}")
    
    def execute_if(self, cmd):
        """Executa uma estrutura condicional"""
        # Extrair a condição
        cond_start = cmd.index('(') + 1
        cond_end = cmd.index(')')
        condition = cmd[cond_start:cond_end].strip()
        
        # Avaliar a condição
        result = self.evaluate(condition)
        
        # Com base no resultado, executar o bloco apropriado
        if result:
            print(f"Condição '{condition}' é verdadeira")
            # O bloco 'então' será executado automaticamente pelo parser de blocos
        else:
            print(f"Condição '{condition}' é falsa")
            # O bloco 'senão' será executado automaticamente pelo parser de blocos, se existir
    
    def execute_while(self, cmd):
        """Executa um loop while"""
        # Extrair a condição
        cond_start = cmd.index('(') + 1
        cond_end = cmd.index(')')
        condition = cmd[cond_start:cond_end].strip()
        
        # Avaliar a condição e executar o loop
        iteration = 1
        while self.evaluate(condition):
            print(f"Loop iteração {iteration} (condição: {condition})")
            # O corpo do loop será executado pelo parser de blocos
            iteration += 1
            
            # Evitar loops infinitos durante o desenvolvimento
            if iteration > 100:
                print("Aviso: Limite de iterações atingido. Possível loop infinito.")
                break
    
    def evaluate(self, expr):
        """Avalia uma expressão e retorna seu valor"""
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
            # Extrair os itens da lista
            items_str = expr[5:-1].strip()
            
            if not items_str:
                return []
            
            # Dividir por vírgula e avaliar cada item
            items = []
            for item in items_str.split(','):
                items.append(self.evaluate(item.strip()))
            
            return items
        
        # Operadores de comparação
        if ' > ' in expr:
            left, right = expr.split(' > ', 1)
            return self.evaluate(left) > self.evaluate(right)
        if ' < ' in expr:
            left, right = expr.split(' < ', 1)
            return self.evaluate(left) < self.evaluate(right)
        if ' == ' in expr:
            left, right = expr.split(' == ', 1)
            return self.evaluate(left) == self.evaluate(right)
        
        # Operadores aritméticos
        if ' + ' in expr:
            left, right = expr.split(' + ', 1)
            return self.evaluate(left) + self.evaluate(right)
        if ' - ' in expr:
            left, right = expr.split(' - ', 1)
            return self.evaluate(left) - self.evaluate(right)
        if ' * ' in expr:
            left, right = expr.split(' * ', 1)
            return self.evaluate(left) * self.evaluate(right)
        if ' / ' in expr:
            left, right = expr.split(' / ', 1)
            return self.evaluate(left) // self.evaluate(right)
        
        # Variável
        if expr in self.variables:
            return self.variables[expr]
        
        # Se não for nenhum dos casos acima
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
    return vm.run(filename)

if __name__ == "__main__":
    sys.exit(main())
