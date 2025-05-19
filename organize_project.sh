#!/bin/bash
# organize_project.sh - Script para organizar os arquivos do projeto para commit

echo "=== Organizando o projeto da Linguagem Peixe para commit ==="
echo

# Criar pasta para arquivos gerados/temporários se não existir
if [ ! -d "gerados" ]; then
    echo "Criando pasta 'gerados' para arquivos temporários..."
    mkdir -p gerados
fi

# Criar pasta para implementações experimentais se não existir
if [ ! -d "experimentos" ]; then
    echo "Criando pasta 'experimentos' para implementações experimentais..."
    mkdir -p experimentos
fi

# Mover arquivos gerados pelo compilador
echo "Movendo arquivos gerados pelo compilador para pasta 'gerados'..."
mv -f *.o lex.yy.c parser.tab.c gerados/ 2>/dev/null
mv -f *.bytecode.json *bytecode.json gerados/ 2>/dev/null

# Mover arquivos experimentais/alternativos
echo "Movendo implementações experimentais para pasta 'experimentos'..."
mv -f peixe_with_python_vm.py peixe_vm_adapter.py peixe_vm_simple_plus.py experimentos/ 2>/dev/null
mv -f compile_peixe.py vm_peixe.py peixe_vm.sh experimentos/ 2>/dev/null

# Mover arquivos relacionados ao LLVM
echo "Movendo arquivos LLVM não finalizados para pasta 'experimentos'..."
mv -f llvm_codegen.c llvm_codegen.h main_llvm.c main.c.llvm parser.y.llvm experimentos/ 2>/dev/null
mv -f parser.tab.llvm.c parser.tab.llvm.h parser.tab.llvm.tab.llvm experimentos/ 2>/dev/null
mv -f Makefile.llvm Makefile.novo parser.y.new experimentos/ 2>/dev/null

# Limpar arquivos de cache Python
echo "Removendo arquivos de cache Python..."
rm -rf python/__pycache__

# Verificar se já existe um arquivo .gitignore
if [ ! -f ".gitignore" ]; then
    echo "Criando arquivo .gitignore..."
    cat > .gitignore << EOF
# Arquivos gerados pelo compilador
*.o
*.a
*.so
*.lo
*.la
lex.yy.c
parser.tab.c
parser.tab.h

# Arquivos de bytecode gerados
*.bytecode.json

# Pastas de arquivos temporários
gerados/
experimentos/

# Cache do Python
__pycache__/
*.py[cod]
*$py.class
EOF
else
    echo "Arquivo .gitignore já existe, não será modificado."
fi

# Listar arquivos essenciais para o projeto
echo
echo "=== Arquivos essenciais mantidos para commit ==="
echo "Arquivos principais:"
ls -la vm_peixe_final.py peixe_vm_run.sh compile_peixe_simple.py vm_peixe_simple.py peixe_vm_simple.sh peixe_vm_with_vars.py README.md apresentacao_peixe.pptx 2>/dev/null

echo
echo "Arquivos de implementação original:"
ls -la lexer.l parser.y parser.tab.h main.c Makefile 2>/dev/null

echo
echo "Implementação Python:"
ls -la python/*.py 2>/dev/null

echo "Programas de exemplo:"
ls -la teste_vm.pesca teste_simples.pesca teste.pesca teste2.pesca teste_avancado.pesca 2>/dev/null

echo
echo "Scripts de suporte:"
ls -la peixe_vm_run.sh peixe_vm_simple.sh test_all.sh organize_project.sh clean_peixe.sh 2>/dev/null

echo
echo "=== Organização concluída! ==="
echo "- Arquivos gerados foram movidos para './gerados/'"
echo "- Implementações experimentais foram movidas para './experimentos/'"
echo "- Foi criado um arquivo .gitignore para evitar o commit de arquivos temporários"
echo
echo "Você pode fazer o commit do projeto agora."
