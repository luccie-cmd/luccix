#include <ast.h>
#include <stdlib.h>
#include <stdio.h>

luccix_assembler_ast_node* createAstNode(luccix_assembler_ast_node_type type){
    luccix_assembler_ast_node* node = malloc(sizeof(node[0]));
    node->type = type;
    return node;
}
void destroyAstNode(luccix_assembler_ast_node* node){
    switch(node->type){
        case AST_NODE_INST: {
            util_da_free(node->inst.operands);
        } break;
        case AST_NODE_NUMBER:
        case AST_NODE_REGISTER:
        case AST_NODE_LABEL_DECL:
        case AST_NODE_LABEL:
        case AST_NODE_INVALID:
        default: {} break;
    }
    free(node);
}
void printAst(luccix_assembler_ast* ast){
    printf("Ast number of nodes  = %ld\n", util_da_count(ast->nodes));
    printf("Ast number of labels = %ld\n", util_da_count(ast->labels));
}

luccix_assembler_ast* createAst(){
    luccix_assembler_ast* ast = malloc(sizeof(ast[0]));
    return ast;
}