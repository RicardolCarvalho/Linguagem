# Linguagem Pesca - Máquina Virtual

Este projeto implementa uma máquina virtual (VM) para interpretar programas escritos na linguagem "peixe" (.pesca).

## Instalação

Não é necessária instalação específica além dos requisitos padrão do Python 3. Todas as implementações utilizam apenas a biblioteca padrão do Python.

### Requisitos
- Python 3.6 ou superior

### Preparação
Clone ou baixe este repositório e dê permissões de execução aos scripts:

```bash
# Dar permissão de execução aos scripts
chmod +x peixe_vm_run.sh
chmod +x peixe_vm_simple.sh
chmod +x vm_peixe_final.py
chmod +x compile_peixe_simple.py
chmod +x vm_peixe_simple.py
chmod +x clean_peixe.sh
chmod +x organize_project.sh
```

## Implementações Disponíveis

O projeto inclui várias implementações da VM com diferentes níveis de funcionalidade:

1. **VM Final (vm_peixe_final.py)**: 
   - Suporte completo para variáveis, listas, condicionais e loops
   - Interpretação direta do código fonte
   - Interface simplificada de linha de comando

2. **VM Simples (peixe_vm_simple.py + compile_peixe_simple.py)**:
   - Compilador que transforma código peixe em bytecode JSON
   - VM que executa o bytecode
   - Funcionalidade básica (atribuições e impressão)

3. **Adaptador Python (peixe_vm_with_vars.py)**:
   - Utiliza a infraestrutura do interpretador Python existente
   - Suporte a declaração automática de variáveis
   - Compatibilidade com a sintaxe da linguagem peixe

## Recursos Suportados

A VM final suporta os seguintes recursos da linguagem peixe:

- **Variáveis** e atribuições (ex: `peixe = 10;`)
- **Tipos de Dados**:
  - Inteiros (ex: `10`)
  - Booleanos (`salmao` para verdadeiro, `tilapia` para falso)
  - Strings (ex: `"Peguei um grande!"`)
  - Listas (ex: `rede[1, 2, 3]`)
- **Estruturas de Controle**:
  - Condicionais (`se`/`senão`)
  - Loops (`enquanto`)
- **Expressões**:
  - Operadores aritméticos (`+`, `-`, `*`, `/`)
  - Operadores de comparação (`>`, `<`, `==`)
- **Saída**: Comando `fale()` para impressão

## Como Usar

Existem várias maneiras de executar programas na linguagem Peixe:

### Guia de Início Rápido

Para testar rapidamente a VM com um exemplo pronto:

```bash
# 1. Dar permissões de execução
chmod +x peixe_vm_run.sh vm_peixe_final.py

# 2. Executar um programa de exemplo
./peixe_vm_run.sh teste_simples.pesca
```

### Escolha sua Implementação

Você pode escolher entre diferentes implementações:

### 1. Usando a VM Final (Recomendado):

A VM final fornece a implementação mais completa:

```bash
# Dê permissão de execução ao script wrapper e à VM
chmod +x peixe_vm_run.sh
chmod +x vm_peixe_final.py

# Execute um programa peixe
./peixe_vm_run.sh programa.pesca
```

Alternativamente, você pode chamar a VM diretamente:

```bash
python3 vm_peixe_final.py programa.pesca
```

### 2. Usando a VM Simples com compilação para bytecode:

Essa abordagem compila o programa para bytecode JSON e depois o executa:

```bash
# Compilar para bytecode
python3 compile_peixe_simple.py programa.pesca programa.bytecode.json

# Executar o bytecode
python3 vm_peixe_simple.py programa.bytecode.json
```

Ou use o script helper:

```bash
./peixe_vm_simple.sh programa.pesca
```

### 3. Usando o Adaptador Python:

Essa implementação utiliza a infraestrutura do interpretador Python existente:

```bash
python3 peixe_vm_with_vars.py programa.pesca
```

## Exemplos

Veja os arquivos de teste incluídos:
- `teste_vm.pesca`: Exemplo simples de atribuição e impressão
- `teste_simples.pesca`: Demonstração de condicionais e loops
- `teste.pesca`: Exemplo com listas e condicionais
- `teste2.pesca`: Exemplo completo com todos os recursos
- `teste_avancado.pesca`: Demonstração abrangente de todos os recursos da linguagem

### Testar com os exemplos incluídos

Para testar a VM com os arquivos de exemplo incluídos:

```bash
# Teste simples (atribuição e impressão)
./peixe_vm_run.sh teste_vm.pesca

# Teste com condicionais e loops
./peixe_vm_run.sh teste_simples.pesca

# Teste com listas e condicionais
./peixe_vm_run.sh teste.pesca

# Teste completo com todos os recursos
./peixe_vm_run.sh teste2.pesca

# Demonstração abrangente de todos os recursos
./peixe_vm_run.sh teste_avancado.pesca
```

Você também pode executar os testes com as outras implementações:

```bash
# Usando a VM simples com compilação para bytecode
./peixe_vm_simple.sh teste_vm.pesca

# Usando o adaptador Python
python3 peixe_vm_with_vars.py teste_vm.pesca
```

### Criando seus próprios programas

Para criar seu próprio programa na linguagem Peixe:

1. Crie um arquivo de texto com extensão `.pesca`
2. Escreva seu código seguindo a sintaxe da linguagem Peixe
3. Execute com a VM:
   ```bash
   ./peixe_vm_run.sh seu_programa.pesca
   ```

Exemplo de um programa simples:
```pesca
// Meu primeiro programa Peixe
contador = 5;

enquanto (contador > 0) {
    fale("Contagem: ");
    fale(contador);
    contador = contador - 1;
}

fale("Fim da contagem!");
```

## Implementação

A VM foi implementada em Python, sem depender de frameworks externos como LLVM ou JVM, oferecendo uma solução leve e portável.

O interpretador final (`vm_peixe_final.py`) implementa:
1. **Análise Léxica**: Processamento do texto do programa em tokens
2. **Parser Recursivo**: Análise da estrutura do programa
3. **Avaliador**: Execução das instruções do programa
4. **Tabela de Símbolos**: Gerenciamento de variáveis e valores

## Detalhes das Implementações

### 1. VM Final (`vm_peixe_final.py`)

Esta é a implementação mais completa, oferecendo as seguintes características:

- **Interpretação Direta**: Lê e executa o código fonte diretamente
- **Análise Léxica**: Divide o texto em tokens através de expressões regulares
- **Parser Recursivo**: Constrói uma representação intermediária do programa
- **Avaliador**: Executa as instruções interpretando a estrutura do programa
- **Tabela de Símbolos**: Mantém um dicionário de variáveis e seus valores

**Uso Recomendado**: Esta é a implementação recomendada para uso geral, pois suporta todos os recursos da linguagem.

### 2. Abordagem com Bytecode (`compile_peixe_simple.py` + `vm_peixe_simple.py`)

Esta implementação divide o processo em duas etapas:

- **Compilador**: Transforma o código fonte em bytecode
  - Gera um arquivo JSON intermediário com instruções simplificadas
  - Realiza uma análise léxica e sintática básica
  - Suporta comandos básicos: atribuições e impressão

- **VM**: Executa o bytecode gerado
  - Lê e interpreta o arquivo JSON de bytecode
  - Mantém uma tabela de símbolos para as variáveis
  - Executa cada instrução sequencialmente

**Uso Recomendado**: Útil para entender o conceito de compilação para código intermediário.

### 3. Adaptador Python (`peixe_vm_with_vars.py`)

Esta implementação funciona como um adaptador:

- **Reutilização**: Aproveita a infraestrutura do interpretador Python existente
- **Compatibilidade**: Adiciona suporte para a sintaxe específica da linguagem Peixe
- **Automação**: Implementa declaração automática de variáveis
- **Integração**: Converte estruturas da linguagem Peixe para equivalentes em Python

**Uso Recomendado**: Alternativa simplificada que facilita a experimentação com a sintaxe da linguagem.

## Recursos Adicionais

- [Guia de Referência Rápida](QUICKREF.md) - Sintaxe e exemplos concisos
- [Exemplos](teste_avancado.pesca) - Demonstração abrangente da linguagem
- [Script de Organização](organize_project.sh) - Script para organizar os arquivos do projeto
- [Script de Limpeza](clean_peixe.sh) - Script para limpar arquivos temporários
- [Apresentação](apresentacao_peixe.pptx) - Slides de apresentação da linguagem

## Manutenção

### Limpeza de Arquivos Temporários

Para limpar todos os arquivos temporários gerados pela VM:

```bash
# Dar permissão de execução ao script de limpeza
chmod +x clean_peixe.sh

# Executar a limpeza
./clean_peixe.sh
```

Isso removerá:
- Arquivos de bytecode (*.bytecode.json)
- Arquivos gerados pelo compilador (*.o, lex.yy.c, etc.)
- Cache Python (__pycache__, *.pyc)

### Organização do Projeto

Para organizar os arquivos do projeto, separando os essenciais dos temporários:
```bash
# Executar o script de organização
./organize_project.sh
```

Isso facilita o versionamento, permitindo que apenas os arquivos importantes sejam controlados pelo sistema de controle de versão.

### Estrutura para Versionamento

Para versionamento, recomenda-se incluir apenas:
- Todos os arquivos `.py` da implementação
- Os scripts shell (`.sh`)
- Arquivos de teste (`.pesca`)
- Documentação (README.md)
- O arquivo `.gitignore` configurado para ignorar arquivos temporários

## Tipos de Valores
- `salmao`: verdadeiro
- `tilapia`: falso
- `rede`: representa uma lista

## Sintaxe em EBNF
```ebnf
PROGRAM     = { STATEMENT } ;
STATEMENT   = ( ASSIGNMENT | PRINT | IF | WHILE ) ";" ;

ASSIGNMENT  = IDENTIFIER , "=" , EXPRESSION ;
PRINT       = "fale" , "(" , EXPRESSION , ")" ;
IF          = "se" , "(" , EXPRESSION , ")" , BLOCK , [ "senão" , BLOCK ] ;
WHILE       = "enquanto" , "(" , EXPRESSION , ")" , BLOCK ;
BLOCK       = "{" , { STATEMENT } , "}" ;

EXPRESSION  = RELATION , { ("++") , RELATION } ;
RELATION    = TERM , [ ("==" | "!=" | ">" | "<" | ">=" | "<=") , TERM ] ;
TERM        = FACTOR , { ("+" | "-") , FACTOR } ;
FACTOR      = PRIMARY , { ("*" | "/") , PRIMARY } ;
PRIMARY     = ( ["+" | "-"] , PRIMARY )
            | NUMBER
            | STRING
            | BOOL
            | LIST_EXPR
            | IDENTIFIER
            | "(" , EXPRESSION , ")" ;

LIST_EXPR   = "rede" , "[" , LIST_ITEMS , "]" ;
LIST_ITEMS  = [ EXPRESSION , { "," , EXPRESSION } ] ;

IDENTIFIER  = LETTER , { LETTER | DIGIT | "_" } ;
NUMBER      = DIGIT , { DIGIT } ;
STRING      = '"' , { CHAR } , '"' ;
BOOL        = "salmao" | "tilapia" ;

LETTER      = "a" | ... | "z" | "A" | ... | "Z" ;
DIGIT       = "0" | "1" | ... | "9" ;
CHAR        = qualquer caractere visível exceto aspas ;
```

## Exemplo de Código
```pesca
peixe = 10;
agua_limpa = salmao;
minha_lista = rede[1, 2, 3];

se (peixe > 5) {
    fale("Peguei um grande!");
} senão {
    fale("Muito pequeno!");
}
```

## Testes e Uso Avançado

### Testes de Unidade

Para testar todos os recursos da VM, você pode executar todos os exemplos incluídos:

```bash
# Criar um script para testar todas as implementações
cat > test_all.sh << EOF
#!/bin/bash
echo "==== Testando todas as implementações ===="

echo
echo "--- Testando VM Final ---"
./peixe_vm_run.sh teste_vm.pesca
./peixe_vm_run.sh teste_simples.pesca
./peixe_vm_run.sh teste.pesca
./peixe_vm_run.sh teste2.pesca

echo
echo "--- Testando VM Simples (Bytecode) ---"
./peixe_vm_simple.sh teste_vm.pesca

echo
echo "--- Testando Adaptador Python ---"
python3 peixe_vm_with_vars.py teste_vm.pesca
EOF

chmod +x test_all.sh
./test_all.sh
```

### Análise de Desempenho

Para comparar o desempenho entre as diferentes implementações:

```bash
# Criar um programa de teste de loop grande
cat > teste_loop.pesca << EOF
contador = 1000000;
soma = 0;

enquanto (contador > 0) {
    soma = soma + contador;
    contador = contador - 1;
}

fale("Resultado: ");
fale(soma);
EOF

# Medir o tempo de execução de cada implementação
time ./peixe_vm_run.sh teste_loop.pesca
time ./peixe_vm_simple.sh teste_loop.pesca
```

### Debugging e Rastreamento

A VM Final inclui recursos de debug que podem ser ativados modificando o arquivo `vm_peixe_final.py`:

```python
# Ativar modo de debug para ver a execução passo a passo
DEBUG = True  # Altere para True no arquivo vm_peixe_final.py

# Ou ative via linha de comando (requer modificação no arquivo da VM)
# python3 vm_peixe_final.py programa.pesca --debug
```

## Desenvolvimento

Para contribuir com o desenvolvimento da VM Peixe:

1. **Ambiente de Desenvolvimento**
   - Recomenda-se o uso de um ambiente virtual Python:
   ```bash
   python -m venv venv
   source venv/bin/activate  # No Linux/Mac
   ```

2. **Estilo de Código**
   - Siga a convenção PEP 8 para código Python
   - Use comentários explicativos para partes complexas do parser/avaliador

3. **Contribuindo com Melhorias**
   - Adicione novos recursos em módulos separados
   - Escreva testes para verificar a implementação
   - Documente novas funcionalidades no README

4. **Fluxo de Trabalho Recomendado**
   - Desenvolva em branches separados
   - Use o script `organize_project.sh` antes de cada commit
   - Mantenha o arquivo `.gitignore` atualizado