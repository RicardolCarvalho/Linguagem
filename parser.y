%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
%}

%union {
    int ival;
    int boolean;
    char* str;
}

%token <ival> TNUMBER
%token <str> TIDENTIFIER TSTRING
%token <boolean> TBOOL

%token TFALE TSE TSENAO TENQUANTO
%token TASSIGN TCONCAT TPLUS TMINUS TMULT TDIV
%token TLPAREN TRPAREN TLBRACE TRBRACE TSEMI

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
      assignment TSEMI
    | print_stmt TSEMI
    | if_stmt
    | while_stmt
    ;

assignment:
    TIDENTIFIER TASSIGN expression
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
    term
    | expression TPLUS term
    | expression TMINUS term
    | expression TCONCAT term
    ;

term:
    factor
    | term TMULT factor
    | term TDIV factor
    ;

factor:
      TNUMBER
    | TSTRING
    | TBOOL
    | TIDENTIFIER
    | TLPAREN expression TRPAREN
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}

