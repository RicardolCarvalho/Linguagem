#ifndef LLVM_CODEGEN_H
#define LLVM_CODEGEN_H

#include <stdbool.h>

// Tipos de valor suportados pela linguagem peixe
typedef enum { 
    TYPE_INT, 
    TYPE_BOOL, 
    TYPE_STRING, 
    TYPE_LIST 
} PeixeType;

// Inicializa o módulo LLVM
void init_llvm_module(const char* module_name);

// Finaliza o módulo LLVM e salva o resultado
void finalize_llvm_module(const char* output_filename);

// Funções para geração de código
void gen_global_variable(const char* name, PeixeType type, void* initial_value);
void gen_int_constant(int value);
void gen_bool_constant(bool value);
void gen_string_constant(const char* value);
void gen_list_constant(int* items, int length);
void gen_binary_op(char op, PeixeType result_type);
void gen_variable_reference(const char* name);
void gen_assignment(const char* name, bool is_declaration);
void gen_print_value(PeixeType type);
void gen_if_condition_start(void);
void gen_if_then_start(void);
void gen_if_else_start(void);
void gen_if_end(void);
void gen_while_condition_start(void);
void gen_while_body_start(void);
void gen_while_end(void);
void gen_comparison(char* op);

// Executa o código LLVM gerado
int execute_llvm_ir(void);

#endif /* LLVM_CODEGEN_H */
