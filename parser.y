%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
extern int yylineno;
extern char* yytext;
%}

%union {
    int ival;
    int boolean;
    char* str;
    void* list;  /* Para suportar o tipo 'rede' (lista) */
}

%token <ival> TNUMBER
%token <str> TIDENTIFIER TSTRING
%token <boolean> TBOOL

%token TFALE TSE TSENAO TENQUANTO
%token TASSIGN TCONCAT TPLUS TMINUS TMULT TDIV
%token TEQ TNEQ TGT TLT TGTE TLTE
%token TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TSEMICOLON
%token TLBRACKET TRBRACKET TREDE
%start program

%%

program:
    statements
    ;

statements:
    /* vazio */
    | statements statement
    ;

statement:
      assignment TSEMICOLON
    | print_stmt TSEMICOLON
    | if_stmt
    | while_stmt
    ;

assignment:
    TIDENTIFIER TASSIGN expression
    {
        printf("Atribuição reconhecida: %s\n", $1);
    }
    ;

print_stmt:
    TFALE TLPAREN expression TRPAREN
    ;

if_stmt:
    TSE TLPAREN expression TRPAREN block
    | TSE TLPAREN expression TRPAREN block TSENAO block
    ;

while_stmt:
    TENQUANTO TLPAREN expression TRPAREN block
    ;

block:
    TLBRACE statements TRBRACE
    ;

expression:
    relation
    | expression TCONCAT relation
    ;

relation:
    term
    | term TEQ term
    | term TNEQ term
    | term TGT term
    | term TLT term
    | term TGTE term
    | term TLTE term
    ;

term:
    factor
    | term TPLUS factor
    | term TMINUS factor
    ;

factor:
    primary
    | factor TMULT primary
    | factor TDIV primary
    ;

primary:
      TPLUS primary
    | TMINUS primary
    | TNUMBER
    | TSTRING
    | TBOOL
    | list_expr
    | TIDENTIFIER
    | TLPAREN expression TRPAREN
    ;

list_expr:
    TREDE TLBRACKET list_items TRBRACKET
    {
        printf("Lista reconhecida\n");
    }
    ;

list_items:
    /* vazio */
    {
        printf("Lista vazia\n");
    }
    | expression
    {
        printf("Lista com um item\n");
    }
    | list_items TCOMMA expression
    {
        printf("Lista com mais um item\n");
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe na linha %d: %s (token: '%s')\n", yylineno, s, yytext);
}