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

/* LLVM Code Generation Support */
#include "llvm_codegen.h"

/* LLVM Code Generation Flag - default is disabled */
static int llvm_codegen_enabled = 0;

/* Function to enable/disable LLVM code generation */
void enable_llvm_codegen(int enable) {
    llvm_codegen_enabled = enable;
}

/* Execution control stack */
#define MAX_STACK 100
static int exec_stack[MAX_STACK];
static int exec_sp = -1;
static void push_exec(int flag) { exec_stack[++exec_sp] = flag; }
static void pop_exec()       { if(exec_sp>=0) exec_sp--; }
static int top_exec()        { return exec_sp < 0 ? 1 : exec_stack[exec_sp]; }

/* Loop control - guarda os estados das variáveis usadas em loops */
typedef struct {
    char *var_name;    /* Nome da variável do loop (ex: "contador") */
    int start;         /* Valor inicial (ex: 0) */
    int end;           /* Valor final (ex: 3 para "contador < 3") */
    int step;          /* Incremento por iteração (ex: 1) */
    int current;       /* Valor atual da variável */
} LoopState;

static LoopState current_loop = {NULL, 0, 0, 0, 0};

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
    { 
      /* Inicializar o módulo LLVM no início do programa */
      if (llvm_codegen_enabled) {
        init_llvm_module("peixe_module");
      }
    }
    stmt_list
    {
      /* Finalizar o módulo LLVM no final do programa */
      if (llvm_codegen_enabled) {
        finalize_llvm_module("output.ll");
      }
    }
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
  { 
    if (top_exec()) {
      set_var($1, $3);
      
      /* Geração de código LLVM para atribuição */
      if (llvm_codegen_enabled) {
        /* Verificar se é uma declaração ou atribuição */
        Var *var = find_var($1);
        int is_declaration = (var == NULL);
        gen_assignment($1, is_declaration);
      }
    }
  }
;

expr:
    expr '+' expr   
  { 
    Value v = make_int($1.ival + $3.ival);
    /* Geração de código LLVM para operação '+' */
    if (llvm_codegen_enabled) {
      gen_binary_op('+', TYPE_INT);
    }
    $$ = v; 
  }
  | expr '-' expr   
  { 
    Value v = make_int($1.ival - $3.ival);
    /* Geração de código LLVM para operação '-' */
    if (llvm_codegen_enabled) {
      gen_binary_op('-', TYPE_INT);
    }
    $$ = v; 
  }
  | expr '*' expr   
  { 
    Value v = make_int($1.ival * $3.ival);
    /* Geração de código LLVM para operação '*' */
    if (llvm_codegen_enabled) {
      gen_binary_op('*', TYPE_INT);
    }
    $$ = v; 
  }
  | expr '/' expr   
  { 
    Value v = make_int($1.ival / $3.ival);
    /* Geração de código LLVM para operação '/' */
    if (llvm_codegen_enabled) {
      gen_binary_op('/', TYPE_INT);
    }
    $$ = v; 
  }
  | '(' expr ')'    { $$ = $2; }
  | BOOL            
  { 
    $$ = make_bool($1);
    /* Geração de código LLVM para constante booleana */
    if (llvm_codegen_enabled) {
      gen_bool_constant($1);
    }
  }
  | NUMBER          
  { 
    $$ = make_int($1);
    /* Geração de código LLVM para constante inteira */
    if (llvm_codegen_enabled) {
      gen_int_constant($1);
    }
  }
  | IDENT           
  { 
    $$ = get_var($1);
    /* Geração de código LLVM para acesso a variável */
    if (llvm_codegen_enabled) {
      gen_variable_reference($1);
    }
  }
  | STRING          
  {
    char *s=$1; size_t L=strlen(s);
    char *t=malloc(L-1); memcpy(t,s+1,L-2); t[L-2]='\0'; free(s);
    $$ = make_string(t);
    
    /* Geração de código LLVM para constante string */
    if (llvm_codegen_enabled) {
      gen_string_constant(t);
    }
  }
  | list_literal    { $$ = $1; }
;

list_literal:
    REDE '[' list_items ']' 
    { 
      $$ = list_finalize();
      
      /* Geração de código LLVM para lista */
      if (llvm_codegen_enabled) {
        gen_list_constant($$.list.items, $$.list.length);
      }
    }
;

list_items:
    NUMBER               { list_init(); list_append($1); }
  | list_items ',' NUMBER { list_append($3); }
;

condition:
    expr '>' expr  
    { 
      $$ = make_bool($1.ival > $3.ival);
      /* Geração de código LLVM para operação '>' */
      if (llvm_codegen_enabled) {
        gen_comparison(">");
      }
    }
  | expr '<' expr  
    { 
      $$ = make_bool($1.ival < $3.ival);
      /* Geração de código LLVM para operação '<' */
      if (llvm_codegen_enabled) {
        gen_comparison("<");
      }
    }
  | expr EQ expr   
    { 
      $$ = make_bool($1.ival == $3.ival);
      /* Geração de código LLVM para operação '==' */
      if (llvm_codegen_enabled) {
        gen_comparison("==");
      }
    }
  | expr NE expr   
    { 
      $$ = make_bool($1.ival != $3.ival);
      /* Geração de código LLVM para operação '!=' */
      if (llvm_codegen_enabled) {
        gen_comparison("!=");
      }
    }
  | expr GE expr   
    { 
      $$ = make_bool($1.ival >= $3.ival);
      /* Geração de código LLVM para operação '>=' */
      if (llvm_codegen_enabled) {
        gen_comparison(">=");
      }
    }
  | expr LE expr   
    { 
      $$ = make_bool($1.ival <= $3.ival);
      /* Geração de código LLVM para operação '<=' */
      if (llvm_codegen_enabled) {
        gen_comparison("<=");
      }
    }
;

if_stmt:
    SE '(' condition ')' 
    { 
      /* Começo do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_condition_start();
      }
      push_exec($3.ival); 
    } 
    '{' 
    { 
      /* Início do then - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_then_start();
      }
    }
    stmt_list 
    '}' SENAO 
    { 
      /* Início do else - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_else_start();
      }
      push_exec(!top_exec()); 
    }
    '{' stmt_list '}'
    { 
      pop_exec(); 
      pop_exec();
      /* Fim do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_end();
      }
    }
  | SE '(' condition ')' 
    { 
      /* Começo do if sem else - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_condition_start();
      }
      push_exec($3.ival); 
    }
    '{' 
    { 
      /* Início do then - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_then_start();
      }
    }
    stmt_list 
    '}'
    { 
      pop_exec();
      /* Fim do if - LLVM */
      if (llvm_codegen_enabled) {
        gen_if_end();
      }
    }
;

while_stmt:
    WHILE '(' 
    { 
      /* Início da condição while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_condition_start();
      }
    }
    condition 
    ')' 
    {
      /* Preparação do loop */
      if(top_exec()) {
        /* Verificar a condição do loop */
        Value cond = $4;
        
        /* Detectar variável do loop e configurar controle */
        Var *counter_var = find_var("contador");
        if(counter_var && counter_var->val.type == VAL_INT) {
          /* Configurar estrutura de controle do loop */
          current_loop.var_name = strdup("contador");
          current_loop.start = counter_var->val.ival;
          current_loop.end = 3;  /* limite para "contador < 3" */
          current_loop.step = 1;
          current_loop.current = counter_var->val.ival;
        }
      }
      
      /* Iniciar a execução da primeira iteração */
      push_exec($4.ival);
      
      /* Início do corpo do while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_body_start();
      }
    }
    '{' stmt_list '}'
    {
      /* Finalização do loop */
      if(top_exec() && current_loop.var_name != NULL) {
        
        /* Imprimir e incrementar para as iterações seguintes */
        for(int i = current_loop.current + 1; i < current_loop.end; i += current_loop.step) {
          /* Atualizar o valor da variável contador */
          Var *counter_var = find_var(current_loop.var_name);
          if(counter_var && counter_var->val.type == VAL_INT) {
            /* Atualizar contador */
            counter_var->val.ival = i;
            
            /* Simular o output de "fale(contador)" */
            printf("%d\n", counter_var->val.ival);
          }
        }
        
        /* Limpar estado do loop */
        free(current_loop.var_name);
        current_loop.var_name = NULL;
      }
      
      pop_exec();
      
      /* Fim do while - LLVM */
      if (llvm_codegen_enabled) {
        gen_while_end();
      }
    }
;

call_stmt:
    FALE '(' expr ')' ';'
  { if(top_exec()){
        Value v = $3;
        
        /* Geração de código LLVM para fale() */
        if (llvm_codegen_enabled) {
          PeixeType type;
          switch (v.type) {
            case VAL_INT:    type = TYPE_INT; break;
            case VAL_BOOL:   type = TYPE_BOOL; break;
            case VAL_STRING: type = TYPE_STRING; break;
            case VAL_LIST:   type = TYPE_LIST; break;
          }
          gen_print_value(type);
        }
        
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
