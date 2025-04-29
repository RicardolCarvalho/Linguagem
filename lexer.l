%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

extern int yylineno;
void yyerror(const char *s);
%}

%%

[ \t]+                        ; // Ignorar espaços e tabs
[\n]                         { yylineno++; } // Contar linhas
"fale"                        { printf("Token: fale\n"); return TFALE; }
"se"                          { printf("Token: se\n"); return TSE; }
"senão"                       { printf("Token: senão\n"); return TSENAO; }
"enquanto"                    { printf("Token: enquanto\n"); return TENQUANTO; }
"salmao"                      { printf("Token: salmao\n"); yylval.boolean = 1; return TBOOL; }
"tilapia"                     { printf("Token: tilapia\n"); yylval.boolean = 0; return TBOOL; }
"rede"                        { printf("Token: rede (palavra-chave)\n"); return TREDE; }

\"([^\\\"]|\\.)*\"            { printf("Token: string '%s'\n", yytext); yylval.str = strdup(yytext); return TSTRING; }
[0-9]+                        { printf("Token: number %s\n", yytext); yylval.ival = atoi(yytext); return TNUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*        { printf("Token: identifier '%s'\n", yytext); yylval.str = strdup(yytext); return TIDENTIFIER; }

"=="                          { printf("Token: ==\n"); return TEQ; }
"!="                          { printf("Token: !=\n"); return TNEQ; }
">"                           { printf("Token: >\n"); return TGT; }
"<"                           { printf("Token: <\n"); return TLT; }
">="                          { printf("Token: >=\n"); return TGTE; }
"<="                          { printf("Token: <=\n"); return TLTE; }
"="                           { printf("Token: =\n"); return TASSIGN; }
"++"                          { printf("Token: ++\n"); return TCONCAT; }
"+"                           { printf("Token: +\n"); return TPLUS; }
"-"                           { printf("Token: -\n"); return TMINUS; }
"*"                           { printf("Token: *\n"); return TMULT; }
"/"                           { printf("Token: /\n"); return TDIV; }
"("                           { printf("Token: (\n"); return TLPAREN; }
")"                           { printf("Token: )\n"); return TRPAREN; }
"{"                           { printf("Token: {\n"); return TLBRACE; }
"}"                           { printf("Token: }\n"); return TRBRACE; }
"["                           { printf("Token: [\n"); return TLBRACKET; }
"]"                           { printf("Token: ]\n"); return TRBRACKET; }
","                           { printf("Token: ,\n"); return TCOMMA; }
";"                           { printf("Token: ;\n"); return TSEMICOLON; }

.                             { 
                                fprintf(stderr, "Erro na linha %d: caractere inesperado '%s'\n", yylineno, yytext); 
                                return -1; 
                              }

%%

int yywrap() { return 1; }
