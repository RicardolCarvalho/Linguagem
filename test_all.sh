#!/bin/bash
# test_all.sh - Script para testar todas as implementações da VM Peixe

echo "==== Testando todas as implementações da Linguagem Peixe ===="

# Verificar se os scripts têm permissão de execução
if [ ! -x "peixe_vm_run.sh" ] || [ ! -x "peixe_vm_simple.sh" ]; then
    echo "Dando permissão de execução aos scripts..."
    chmod +x peixe_vm_run.sh peixe_vm_simple.sh vm_peixe_final.py compile_peixe_simple.py vm_peixe_simple.py
fi

echo
echo "--- Testando VM Final ---"
echo "Teste básico (atribuição/impressão):"
./peixe_vm_run.sh teste_vm.pesca

echo
echo "Teste com condicionais e loops:"
./peixe_vm_run.sh teste_simples.pesca

echo
echo "Teste com listas e condicionais:"
./peixe_vm_run.sh teste.pesca

echo
echo "Teste avançado:"
./peixe_vm_run.sh teste_avancado.pesca

echo
echo "--- Testando VM Simples (Bytecode) ---"
./peixe_vm_simple.sh teste_vm.pesca

echo
echo "--- Testando Adaptador Python ---"
python3 peixe_vm_with_vars.py teste_vm.pesca

echo
echo "==== Testes concluídos! ===="
echo "Verificação rápida de todas as implementações concluída."
echo "Para análise de desempenho ou testes mais detalhados, consulte o README.md"
