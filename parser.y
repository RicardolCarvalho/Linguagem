/* parser.y */

%code requires {
    typedef enum { VAL_INT, VAL_BOOL, VAL_LIST, VAL_STRING } ValueType;
    typedef struct {
        ValueType type;
        union {
            int    ival;
            char  *sval;
            struct { int *items; int length; } list;
        };
    } Value;
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

/* Execution control stack */
#define MAX_STACK 100
static int exec_stack[MAX_STACK];
static int exec_sp = -1;
static void push_exec(int flag) { exec_stack[++exec_sp] = flag; }
static void pop_exec()       { if(exec_sp>=0) exec_sp--; }
static int top_exec()        { return exec_sp < 0 ? 1 : exec_stack[exec_sp]; }

/* Symbol table */
typedef struct Var { char *name; Value val; struct Var *next; } Var;
static Var *sym_table = NULL;

/* Helpers */
static Value make_int(int x)    { Value v = {.type=VAL_INT,  .ival = x}; return v; }
static Value make_bool(int b)   { Value v = {.type=VAL_BOOL, .ival = b}; return v; }
static Value make_string(char *s){ Value v = {.type=VAL_STRING, .sval = s}; return v; }
static Value make_list(int *a,int n){ Value v = {.type=VAL_LIST, .list = {a,n}}; return v; }
static void free_value(Value v){ if(v.type==VAL_STRING) free(v.sval); else if(v.type==VAL_LIST) free(v.list.items); }

static Var *find_var(const char *name){ for(Var *p=sym_table; p; p=p->next) if(!strcmp(p->name,name)) return p; return NULL; }
static void declare_var(char *name, Value v){ if(find_var(name)){ fprintf(stderr,"Erro: variavel '%s' ja declarada\n",name); exit(EXIT_FAILURE); } Var *p=malloc(sizeof(Var)); p->name=name; p->val=v; p->next=sym_table; sym_table=p; }
static void assign_var(char *name, Value v){ Var *p=find_var(name); if(!p){ fprintf(stderr,"Erro: variavel '%s' nao definida\n",name); exit(EXIT_FAILURE);} if(p->val.type!=v.type){ fprintf(stderr,"Erro: tipo incompativel na atribuicao '%s'\n",name); exit(EXIT_FAILURE);} free(name); free_value(p->val); p->val=v; }
static void set_var(char *name, Value v){ if(find_var(name)) assign_var(name,v); else declare_var(name,v); }
static Value get_var(char *name){ Var *p=find_var(name); if(!p){ fprintf(stderr,"Erro: variavel '%s' nao definida\n",name); free(name); exit(EXIT_FAILURE);} free(name); return p->val; }

/* List literal support */
static int *list_buf;
static int   list_size, list_cap;
static void list_init(){ list_cap=4; list_size=0; list_buf=malloc(list_cap*sizeof(int)); }
static void list_append(int x){ if(list_size>=list_cap){ list_cap*=2; list_buf=realloc(list_buf,list_cap*sizeof(int)); } list_buf[list_size++]=x; }
static Value list_finalize(){ int *a=malloc(list_size*sizeof(int)); memcpy(a,list_buf,list_size*sizeof(int)); free(list_buf); return make_list(a,list_size); }

int yylex(void);
int yyerror(const char *s){ fprintf(stderr,"Erro de sintaxe: %s\n",s); return 0; }
%}

/* Semantic types */
%union { Value val; int ival; char *sval; }

/* Tokens */
%token <ival> NUMBER BOOL
%token <sval> IDENT STRING
%token        SE SENAO FALE REDE WHILE
%token        EQ GE LE NE

/* Non-terminals */
%type <val> expr condition list_literal

/* Operator precedence */
%left '>' '<' EQ NE GE LE
%left '+' '-'
%left '*' '/'

%%

program:
    stmt_list
;

stmt_list:
    stmt_list stmt
  | /* empty */
;

stmt:
    assignment
  | if_stmt
  | while_stmt
  | call_stmt
;

assignment:
    IDENT '=' expr ';'
  { if(top_exec()) set_var($1, $3); }
;

expr:
    expr '+' expr   { Value v = make_int($1.ival + $3.ival); $$=v; }
  | expr '-' expr   { Value v = make_int($1.ival - $3.ival); $$=v; }
  | expr '*' expr   { Value v = make_int($1.ival * $3.ival); $$=v; }
  | expr '/' expr   { Value v = make_int($1.ival / $3.ival); $$=v; }
  | '(' expr ')'    { $$ = $2; }
  | BOOL            { $$ = make_bool($1); }
  | NUMBER          { $$ = make_int($1); }
  | IDENT           { $$ = get_var($1); }
  | STRING          {
        char *s=$1; size_t L=strlen(s);
        char *t=malloc(L-1); memcpy(t,s+1,L-2); t[L-2]='\0'; free(s);
        $$ = make_string(t);
    }
  | list_literal    { $$ = $1; }
;

list_literal:
    REDE '[' list_items ']' { $$ = list_finalize(); }
;

list_items:
    NUMBER               { list_init(); list_append($1); }
  | list_items ',' NUMBER { list_append($3); }
;

condition:
    expr '>' expr  { $$ = make_bool($1.ival >  $3.ival); }
  | expr '<' expr  { $$ = make_bool($1.ival <  $3.ival); }
  | expr EQ expr   { $$ = make_bool($1.ival == $3.ival); }
  | expr NE expr   { $$ = make_bool($1.ival != $3.ival); }
  | expr GE expr   { $$ = make_bool($1.ival >= $3.ival); }
  | expr LE expr   { $$ = make_bool($1.ival <= $3.ival); }
;

if_stmt:
    SE '(' condition ')' '{' { push_exec($3.ival); } stmt_list '}'
    SENAO '{'                { push_exec(!top_exec()); } stmt_list '}'
    { pop_exec(); pop_exec(); }
  | SE '(' condition ')' '{' { push_exec($3.ival); } stmt_list '}'
    { pop_exec(); }
;

while_stmt:
    WHILE '(' condition ')' '{' { /* enter loop */ }
    stmt_list
    '}'
    {
      /* Loop runtime handled elsewhere */
    }
;

call_stmt:
    FALE '(' expr ')' ';'
  { if(top_exec()){
        Value v = $3;
        switch(v.type) {
          case VAL_INT:    printf("%d\n",v.ival); break;
          case VAL_BOOL:   printf(v.ival?"true\n":"false\n"); break;
          case VAL_STRING: printf("%s\n",v.sval); free(v.sval); break;
          case VAL_LIST:   {
            printf("[");
            for(int i=0;i<v.list.length;i++){
                printf("%d",v.list.items[i]);
                if(i+1<v.list.length) printf(", ");
            }
            printf("]\n"); free(v.list.items);
          } break;
        }
    }}
;

%%

/* End of parser.y */
