#include "llvm_codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mapeamento entre tipos ValueType do parser e tipos PeixeType da nossa VM
#define MAP_TYPE(vt) ((vt) == 0 ? TYPE_INT : \
                      (vt) == 1 ? TYPE_BOOL : \
                      (vt) == 2 ? TYPE_LIST : TYPE_STRING)

// Simulação da interface para LLVM C API
// Em uma implementação real, seriam adicionados os includes do LLVM:
// #include <llvm-c/Core.h>
// #include <llvm-c/ExecutionEngine.h>
// #include <llvm-c/Target.h>
// #include <llvm-c/Analysis.h>
// #include <llvm-c/BitWriter.h>

// Estruturas simplificadas para representar elementos LLVM
typedef struct {
    char* name;
    void* data;
} LLVMModule;

typedef struct {
    char* opcode;
    void* operands[3];
    int operand_count;
} LLVMInstruction;

typedef struct {
    int inst_count;
    int inst_capacity;
    LLVMInstruction** instructions;
} LLVMBasicBlock;

typedef struct {
    char* name;
    LLVMBasicBlock** blocks;
    int block_count;
    int block_capacity;
} LLVMFunction;

// Variáveis globais para o módulo atual
static LLVMModule* current_module = NULL;
static LLVMFunction* current_function = NULL;
static LLVMBasicBlock* current_block = NULL;

// Simulação de instruções LLVM para a linguagem peixe
void init_llvm_module(const char* module_name) {
    if (current_module) {
        fprintf(stderr, "Erro: Módulo LLVM já inicializado\n");
        return;
    }
    
    current_module = (LLVMModule*)malloc(sizeof(LLVMModule));
    current_module->name = strdup(module_name);
    
    // Criar função principal (main)
    current_function = (LLVMFunction*)malloc(sizeof(LLVMFunction));
    current_function->name = strdup("main");
    current_function->block_capacity = 10;
    current_function->block_count = 1;
    current_function->blocks = (LLVMBasicBlock**)malloc(
        current_function->block_capacity * sizeof(LLVMBasicBlock*));
    
    // Criar bloco básico inicial
    current_block = (LLVMBasicBlock*)malloc(sizeof(LLVMBasicBlock));
    current_block->inst_capacity = 100;
    current_block->inst_count = 0;
    current_block->instructions = (LLVMInstruction**)malloc(
        current_block->inst_capacity * sizeof(LLVMInstruction*));
    
    current_function->blocks[0] = current_block;
    
    printf("Inicializado módulo LLVM: %s\n", module_name);
}

void finalize_llvm_module(const char* output_filename) {
    if (!current_module) {
        fprintf(stderr, "Erro: Nenhum módulo LLVM inicializado\n");
        return;
    }
    
    // Adicionar instrução de retorno
    LLVMInstruction* ret_inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    ret_inst->opcode = strdup("ret");
    ret_inst->operand_count = 1;
    ret_inst->operands[0] = (void*)0; // Retornar 0
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = ret_inst;
    }
    
    // Em uma implementação real, este código geraria um arquivo de bytecode LLVM
    printf("Gerando bytecode LLVM para: %s\n", output_filename);
    
    // Simular geração de um arquivo .bc
    FILE* f = fopen(output_filename, "w");
    if (f) {
        fprintf(f, "; Bytecode LLVM gerado para a linguagem peixe\n");
        fprintf(f, "; Módulo: %s\n\n", current_module->name);
        
        fprintf(f, "define i32 @main() {\n");
        
        // Escrever instruções simuladas
        for (int i = 0; i < current_block->inst_count; i++) {
            LLVMInstruction* inst = current_block->instructions[i];
            fprintf(f, "  %s", inst->opcode);
            
            for (int j = 0; j < inst->operand_count; j++) {
                if (j == 0) fprintf(f, " ");
                else fprintf(f, ", ");
                
                // Tratamento simplificado para operandos
                if (inst->operands[j] == NULL) {
                    fprintf(f, "null");
                } else {
                    fprintf(f, "%p", inst->operands[j]);
                }
            }
            
            fprintf(f, "\n");
        }
        
        fprintf(f, "  ret i32 0\n");
        fprintf(f, "}\n");
        
        fclose(f);
        printf("Arquivo de bytecode LLVM gerado: %s\n", output_filename);
    } else {
        fprintf(stderr, "Erro ao criar arquivo de bytecode: %s\n", output_filename);
    }
    
    // Liberar recursos
    // Em uma implementação real, usaríamos as funções do LLVM
    // como LLVMDisposeModule, etc.
    free(current_module->name);
    free(current_module);
    current_module = NULL;
    
    // Limpeza adicional seria necessária na implementação real
}

void gen_int_constant(int value) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("const_int");
    inst->operand_count = 1;
    inst->operands[0] = (void*)(intptr_t)value;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_bool_constant(bool value) {
    gen_int_constant(value ? 1 : 0);
}

void gen_string_constant(const char* value) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("const_string");
    inst->operand_count = 1;
    inst->operands[0] = strdup(value);
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_list_constant(int* items, int length) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("const_list");
    inst->operand_count = 2;
    
    // Criar cópia dos itens
    int* items_copy = (int*)malloc(length * sizeof(int));
    memcpy(items_copy, items, length * sizeof(int));
    
    inst->operands[0] = items_copy;
    inst->operands[1] = (void*)(intptr_t)length;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_binary_op(char op, PeixeType result_type) {
    if (!current_block) return;
    
    char opcode[10];
    switch (op) {
        case '+': strcpy(opcode, "add"); break;
        case '-': strcpy(opcode, "sub"); break;
        case '*': strcpy(opcode, "mul"); break;
        case '/': strcpy(opcode, "div"); break;
        default:  strcpy(opcode, "unknown"); break;
    }
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup(opcode);
    inst->operand_count = 1;
    inst->operands[0] = (void*)(intptr_t)result_type;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_variable_reference(const char* name) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("load");
    inst->operand_count = 1;
    inst->operands[0] = strdup(name);
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_assignment(const char* name, bool is_declaration) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup(is_declaration ? "alloca_and_store" : "store");
    inst->operand_count = 1;
    inst->operands[0] = strdup(name);
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_print_value(PeixeType type) {
    if (!current_block) return;
    
    const char* print_func = NULL;
    
    switch (type) {
        case TYPE_INT:    print_func = "print_int"; break;
        case TYPE_BOOL:   print_func = "print_bool"; break;
        case TYPE_STRING: print_func = "print_string"; break;
        case TYPE_LIST:   print_func = "print_list"; break;
    }
    
    if (print_func) {
        LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
        inst->opcode = strdup("call");
        inst->operand_count = 1;
        inst->operands[0] = strdup(print_func);
        
        if (current_block->inst_count < current_block->inst_capacity) {
            current_block->instructions[current_block->inst_count++] = inst;
        }
    }
}

void gen_if_condition_start(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("br_condition");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_if_then_start(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("then_block");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_if_else_start(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("else_block");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_if_end(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("if_end");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_while_condition_start(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("while_cond");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_while_body_start(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("while_body");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_while_end(void) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("while_end");
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

void gen_comparison(char* op) {
    if (!current_block) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    
    if (strcmp(op, ">") == 0) inst->opcode = strdup("icmp_gt");
    else if (strcmp(op, "<") == 0) inst->opcode = strdup("icmp_lt");
    else if (strcmp(op, "==") == 0) inst->opcode = strdup("icmp_eq");
    else if (strcmp(op, "!=") == 0) inst->opcode = strdup("icmp_ne");
    else if (strcmp(op, ">=") == 0) inst->opcode = strdup("icmp_ge");
    else if (strcmp(op, "<=") == 0) inst->opcode = strdup("icmp_le");
    else inst->opcode = strdup("icmp_unknown");
    
    inst->operand_count = 0;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}

// Execução do código LLVM compilado (simulação)
int execute_llvm_ir(void) {
    printf("\n--- Executando código LLVM ---\n");
    
    // Nesta simulação, apenas imprimimos as instruções que seriam executadas
    for (int i = 0; i < current_block->inst_count; i++) {
        LLVMInstruction* inst = current_block->instructions[i];
        printf("Executando: %s", inst->opcode);
        
        for (int j = 0; j < inst->operand_count; j++) {
            if (j == 0) printf(" ");
            else printf(", ");
            
            // Tratamento simplificado para operandos
            if (inst->operands[j] == NULL) {
                printf("null");
            } else if (strcmp(inst->opcode, "const_string") == 0 && j == 0) {
                printf("\"%s\"", (char*)inst->operands[j]);
            } else if (strcmp(inst->opcode, "load") == 0 && j == 0) {
                printf("%s", (char*)inst->operands[j]);
            } else if (strcmp(inst->opcode, "store") == 0 && j == 0) {
                printf("%s", (char*)inst->operands[j]);
            } else if (strcmp(inst->opcode, "alloca_and_store") == 0 && j == 0) {
                printf("%s", (char*)inst->operands[j]);
            } else if (strcmp(inst->opcode, "call") == 0 && j == 0) {
                printf("%s()", (char*)inst->operands[j]);
            } else {
                printf("%p", inst->operands[j]);
            }
        }
        
        printf("\n");
    }
    
    printf("--- Fim da execução ---\n");
    return 0;
}

// Função para criar variáveis globais
void gen_global_variable(const char* name, PeixeType type, void* initial_value) {
    if (!current_module) return;
    
    LLVMInstruction* inst = (LLVMInstruction*)malloc(sizeof(LLVMInstruction));
    inst->opcode = strdup("global");
    inst->operand_count = 3;
    inst->operands[0] = strdup(name);
    inst->operands[1] = (void*)(intptr_t)type;
    inst->operands[2] = initial_value;
    
    if (current_block->inst_count < current_block->inst_capacity) {
        current_block->instructions[current_block->inst_count++] = inst;
    }
}
