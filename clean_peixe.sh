#!/bin/bash
# clean_peixe.sh - Script para limpar arquivos temporários da VM Peixe

echo "=== Limpando arquivos temporários da Linguagem Peixe ==="

# Remover arquivos gerados pelo compilador
echo "Removendo arquivos gerados pelo compilador..."
rm -f *.o lex.yy.c parser.tab.c parser.tab.h 2>/dev/null

# Remover arquivos de bytecode 
echo "Removendo arquivos de bytecode..."
rm -f *.bytecode.json *bytecode.json 2>/dev/null

# Limpar diretório de arquivos gerados
echo "Limpando diretório de arquivos gerados..."
rm -rf gerados/* 2>/dev/null

# Remover cache Python
echo "Removendo arquivos de cache Python..."
rm -rf __pycache__ python/__pycache__ 2>/dev/null
find . -name "*.pyc" -delete 2>/dev/null

echo "=== Limpeza concluída! ==="
echo "Todos os arquivos temporários foram removidos."
