#!/usr/bin/env bash
# peixe_vm.sh - Script para executar programas da linguagem Peixe

# Verificar argumentos
if [ $# -lt 1 ]; then
    echo "Uso: $0 <arquivo_fonte.pesca>"
    exit 1
fi

SOURCE_FILE="$1"

# Verificar se o arquivo fonte existe
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Erro: Arquivo fonte não encontrado: $SOURCE_FILE"
    exit 1
fi

echo "=========================="
echo "VM PEIXE - Máquina Virtual para a Linguagem Peixe"
echo "=========================="

# Usar o interpretador direto (mais recente)
echo "Executando $SOURCE_FILE com a VM Peixe..."
python3 "$(dirname "$0")/vm_peixe_final.py" "$SOURCE_FILE"
    exit 1
fi

# Tornar os scripts Python executáveis
chmod +x ./compile_peixe.py
chmod +x ./vm_peixe.py

# Compilar o programa
echo "Compilando $SOURCE_FILE para bytecode..."
./compile_peixe.py "$SOURCE_FILE" "$BYTECODE_FILE"

if [ $? -ne 0 ]; then
    echo "Erro durante a compilação!"
    exit 1
fi

# Executar apenas se não for somente compilação
if [ $ONLY_COMPILE -eq 0 ]; then
    echo -e "\nExecutando programa em bytecode..."
    ./vm_peixe.py "$BYTECODE_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Erro durante a execução!"
        exit 1
    fi
fi

exit 0
