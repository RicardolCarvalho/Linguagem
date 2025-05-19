# Linguagem Pesca 🐟

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

### Preparação
Clone ou baixe este repositório e dê permissões de execução aos scripts:

```bash
chmod +x peixe_vm_run.sh
chmod +x peixe_vm_simple.sh
chmod +x vm_peixe_final.py
chmod +x compile_peixe_simple.py
chmod +x vm_peixe_simple.py
chmod +x clean_peixe.sh
chmod +x organize_project.sh
```

## Implementações

 - **VM Final (vm_peixe_final.py)**: 
   - Suporte completo para variáveis, listas, condicionais e loops
   - Interpretação direta do código fonte
   - Interface simplificada de linha de comando

## Como Usar

Existem várias maneiras de executar programas na linguagem Peixe:

### Guia de Início Rápido

Para testar rapidamente a VM com um exemplo pronto:

```bash
chmod +x peixe_vm_run.sh vm_peixe_final.py

./peixe_vm_run.sh teste.pesca
```

### Escolha sua Implementação

Você pode escolher entre diferentes implementações:

### Usando a VM Final:

```bash
chmod +x peixe_vm_run.sh
chmod +x vm_peixe_final.py

./peixe_vm_run.sh programa.pesca
```

Alternativamente, você pode chamar a VM diretamente:

```bash
python3 vm_peixe_final.py programa.pesca
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
./peixe_vm_run.sh teste_vm.pesca
./peixe_vm_run.sh teste_simples.pesca
./peixe_vm_run.sh teste.pesca
./peixe_vm_run.sh teste2.pesca
./peixe_vm_run.sh teste_avancado.pesca
```