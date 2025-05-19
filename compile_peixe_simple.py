#!/usr/bin/env python3
# compile_peixe_simple.py - Compilador simplificado para a linguagem Peixe

import sys
import json
import re

def simple_compile(source):
    # Lista para armazenar as instruções geradas
    bytecode = []
    
    # Processamento linha a linha
    for line in source.strip().split('\n'):
        line = line.strip()
        
        # Pular linhas vazias e comentários
        if not line or line.startswith('//'):
            continue
        
        # Atribuição
        if '=' in line and not ('==' in line or '!=' in line or '<=' in line or '>=' in line):
            # Remover ponto e vírgula, se houver
            if line.endswith(';'):
                line = line[:-1].strip()
            
            parts = line.split('=', 1)
            var_name = parts[0].strip()
            value_str = parts[1].strip()
            
            # Tentar converter para inteiro
            try:
                value = int(value_str)
            except ValueError:
                # Se não for inteiro, verificar se é salmao/tilapia
                if value_str == 'salmao':
                    value = True
                elif value_str == 'tilapia':
                    value = False
                # Verificar se é uma lista (rede[...])
                elif value_str.startswith('rede[') and value_str.endswith(']'):
                    # Extrair conteúdo dentro dos colchetes
                    list_content = value_str[5:-1].strip()
                    items = []
                    
                    # Se a lista não estiver vazia
                    if list_content:
                        # Dividir por vírgulas
                        for item_str in list_content.split(','):
                            item_str = item_str.strip()
                            try:
                                # Converter para inteiro
                                item = int(item_str)
                                items.append(item)
                            except ValueError:
                                print(f"Erro: Item de lista não é um inteiro: {item_str}")
                                return None
                    
                    value = items
                else:
                    # Tratar como string se ainda tiver aspas
                    if value_str.startswith('"') and value_str.endswith('"'):
                        value = value_str[1:-1]
                    else:
                        print(f"Erro: Valor desconhecido na atribuição: {value_str}")
                        return None
            
            # Adicionar instrução de atribuição
            bytecode.append({
                "op": "ASSIGN",
                "var": var_name,
                "value": value
            })
        
        # Chamada de função fale()
        elif line.startswith('fale(') and line.endswith(');'):
            # Extrair nome da variável dentro dos parênteses
            match = re.search(r'fale\(([^)]+)\);', line)
            if match:
                var_name = match.group(1).strip()
                
                # Adicionar instrução de impressão
                bytecode.append({
                    "op": "PRINT",
                    "var": var_name
                })
            else:
                print(f"Erro: Sintaxe inválida para fale(): {line}")
                return None
        
        else:
            print(f"Erro: Sintaxe não reconhecida: {line}")
            return None
    
    return bytecode

def main():
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <arquivo_fonte.pesca> [saida.json]")
        return 1
    
    # Define o nome do arquivo de saída
    input_file = sys.argv[1]
    if len(sys.argv) > 2:
        output_file = sys.argv[2]
    else:
        output_file = input_file + ".bytecode.json"
    
    print(f"Compilando {input_file} para {output_file}")
    
    # Lê o arquivo fonte
    try:
        with open(input_file, 'r') as f:
            source = f.read()
            print(f"Arquivo fonte lido: {len(source)} bytes")
    except Exception as e:
        print(f"Erro ao ler o arquivo fonte: {e}")
        return 1
    
    # Compila o código fonte
    bytecode = simple_compile(source)
    if bytecode is None:
        print("Erro durante a compilação!")
        return 1
    
    print(f"Instruções geradas: {len(bytecode)}")
    
    # Salva o bytecode em um arquivo JSON
    try:
        with open(output_file, 'w') as f:
            json.dump(bytecode, f, indent=2)
        print(f"Bytecode gerado com sucesso: {output_file}")
    except Exception as e:
        print(f"Erro ao salvar o bytecode: {e}")
        return 1
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
