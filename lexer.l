/* lexer.l */
%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

digit   [0-9]
letter  [a-zA-Z_]
id      {letter}({letter}|{digit})*

%%

"se"             return SE;
"senão"          return SENAO;
"enquanto"       return WHILE;
"fale"           return FALE;
"rede"           return REDE;

"salmao"         { yylval.ival = 1; return BOOL; }
"tilapia"        { yylval.ival = 0; return BOOL; }

[ \t\r\n]+       ;                /* ignora espaços em branco */
"//".*           ;                /* ignora comentários de linha */

{digit}+         { yylval.ival = atoi(yytext); return NUMBER; }

\"([^\\\"]|\\.)*\"   {
                    yylval.sval = strdup(yytext);
                    return STRING;
                   }

{id}              { yylval.sval = strdup(yytext); return IDENT; }

">="             return GE;
"<="             return LE;
"=="             return EQ;
"!="             return NE;
">"              return '>';
"<"              return '<';

"+"    return '+';
"-"    return '-';
"*"    return '*';
"/"    return '/';
"="              return '=';
";"              return ';';
","              return ',';
"["              return '[';
"]"              return ']';
"("              return '(';
")"              return ')';
"{"              return '{';
"}"              return '}';

.                 { fprintf(stderr, "Caracter inválido: %s\n", yytext); }

%%
