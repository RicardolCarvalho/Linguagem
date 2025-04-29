# Linguagem de Pesca 🐟

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