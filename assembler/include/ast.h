#pragma once
#include "util.h"
#include <stddef.h>

typedef enum luccix_assembler_ast_node_type{
    AST_NODE_INVALID,
    AST_NODE_LABEL,
    AST_NODE_LABEL_DECL,
    AST_NODE_INST,

    AST_NODE_REGISTER,
    AST_NODE_NUMBER
} luccix_assembler_ast_node_type;

// NOTE: These are not the specified input registers these are for the output, the real one is in a string
typedef enum luccix_assembler_ast_register{
    RAX,
    RBX,
    RCX,
    RDX,
} luccix_assembler_ast_register;

typedef enum luccix_assembler_ast_instruction_type{
    INSTRUCTION_MOV,
    INSTRUCTION_SYSCALL,
} luccix_assembler_ast_instruction_type;

typedef enum luccix_assembler_ast_label_decl_type{
    LABEL_DECL_INVALID,
    LABEL_DECL_EXTERN,
    LABEL_DECL_LOCAL,
    LABEL_DECL_GLOBAL,
} luccix_assembler_ast_label_decl_type;

typedef struct luccix_assembler_ast_node_label{
    luccix_assembler_ast_label_decl_type type;
    const char* name;
    int64_t offset;
} luccix_assembler_ast_node_label;

typedef struct luccix_assembler_ast_node{
    luccix_assembler_ast_node_type type;
    union {
        struct{
            const char* name;
            int64_t offset;
        } label;
        struct{
            luccix_assembler_ast_label_decl_type type;
            const char* name;
        } label_decl;
        struct{
            luccix_assembler_ast_instruction_type type;
            util_da(struct luccix_assembler_ast_node*) operands;
        } inst;
        struct{
            luccix_assembler_ast_register outReg;
            const char* name;
        } Register;
        struct{
            int64_t number;
        } number;
    };
    
} luccix_assembler_ast_node;
typedef struct luccix_assembler_ast{
    // This field should be filled in by the semantic analyser
    util_da(luccix_assembler_ast_node_label*) labels;
    util_da(luccix_assembler_ast_node*) nodes;
} luccix_assembler_ast;


luccix_assembler_ast* createAst();
void destroyAst(luccix_assembler_ast* ast);
void printAst(luccix_assembler_ast* ast);
luccix_assembler_ast_node* createAstNode(luccix_assembler_ast_node_type type);
void destroyAstNode(luccix_assembler_ast_node* node);