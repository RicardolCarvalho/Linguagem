%{
#include "parser.tab.h"
#include <string.h>

int yylineno;
void yyerror(const char *s);
%}

%%
[ \t\r\n]+                    ; // Ignorar espaços e quebras de linha
"fale"                        { return TFALE; }
"se"                          { return TSE; }
"senão"                       { return TSENAO; }
"enquanto"                    { return TENQUANTO; }
"verdadeiro"                  { yylval.boolean = 1; return TBOOL; }
"falso"                       { yylval.boolean = 0; return TBOOL; }

\"([^\\\"]|\\.)*\"            { yylval.str = strdup(yytext); return TSTRING; }
[0-9]+                        { yylval.ival = atoi(yytext); return TNUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*        { yylval.str = strdup(yytext); return TIDENTIFIER; }

"="                           { return TASSIGN; }
"++"                          { return TCONCAT; }
"+"                           { return TPLUS; }
"-"                           { return TMINUS; }
"*"                           { return TMULT; }
"/"                           { return TDIV; }
"("                           { return TLPAREN; }
")"                           { return TRPAREN; }
"{"                           { return TLBRACE; }
"}"                           { return TRBRACE; }
";"                           { return TSEMI; }

.                             { printf("Caractere inesperado: %s\n", yytext); return -1; }

%%
int yywrap() { return 1; }
