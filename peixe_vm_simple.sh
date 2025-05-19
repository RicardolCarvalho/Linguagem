#!/usr/bin/env bash
# peixe_vm_simple.sh - Script para compilar e executar programas da linguagem Peixe (versão simples)

# Verificar argumentos
if [ $# -lt 1 ]; then
    echo "Uso: $0 <arquivo_fonte.pesca> [--somente-compilar]"
    exit 1
fi

SOURCE_FILE="$1"
BYTECODE_FILE="${SOURCE_FILE}.bytecode.json"
ONLY_COMPILE=0

# Verificar se o usuário quer apenas compilar
if [ "$2" = "--somente-compilar" ]; then
    ONLY_COMPILE=1
fi

# Verificar se o arquivo fonte existe
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Erro: Arquivo fonte não encontrado: $SOURCE_FILE"
    exit 1
fi

# Tornar os scripts Python executáveis
chmod +x ./compile_peixe_simple.py
chmod +x ./vm_peixe_simple.py

# Compilar o programa
echo "Compilando $SOURCE_FILE para bytecode..."
python3 ./compile_peixe_simple.py "$SOURCE_FILE" "$BYTECODE_FILE"

if [ $? -ne 0 ]; then
    echo "Erro durante a compilação!"
    exit 1
fi

# Executar apenas se não for somente compilação
if [ $ONLY_COMPILE -eq 0 ]; then
    echo -e "\nExecutando programa em bytecode..."
    python3 ./vm_peixe_simple.py "$BYTECODE_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Erro durante a execução!"
        exit 1
    fi
fi

exit 0
