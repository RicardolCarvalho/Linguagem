#!/usr/bin/env bash
# peixe_vm_run.sh - Script simplificado para executar programas da linguagem Peixe

if [ $# -lt 1 ]; then
    echo "Uso: $0 <arquivo.pesca>"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "Erro: Arquivo não encontrado: $1"
    exit 1
fi

echo "========================================"
echo "VM PEIXE - Executando $1"
echo "========================================"

python3 vm_peixe_final.py "$1"
