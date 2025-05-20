/* Stub implementation for LLVM codegen - LLVM support is disabled */
#include "llvm_codegen.h"

/* Stub implementations - do nothing */
void init_llvm_module(const char *name) {}
void finalize_llvm_module(const char *output) {}
void gen_assignment(char *name, int is_declaration) {}
void gen_binary_op(char op, PeixeType type) {}
void gen_bool_constant(int value) {}
void gen_int_constant(int value) {}
void gen_variable_reference(char *name) {}
void gen_string_constant(char *value) {}
void gen_list_constant(int *items, int length) {}
void gen_comparison(const char *op) {}
void gen_if_condition_start(void) {}
void gen_if_then_start(void) {}
void gen_if_else_start(void) {}
void gen_if_end(void) {}
void gen_while_condition_start(void) {}
void gen_while_body_start(void) {}
void gen_while_end(void) {}
void gen_print_value(PeixeType type) {}
