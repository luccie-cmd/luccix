#include <parser.h>
#include <stdlib.h>
#include <stdio.h>

static void parserAdvance(luccix_assembler_parser* parser){
    parser->diag->addTrace(parser->diag, __func__);
    if(util_da_count(parser->lineTokens) > parser->tokenIdx){
        parser->diag->print(parser->diag, DIAG_LEVEL_ERROR, "Expected a token but got a newline\n");
        parser->status = PARSER_STATUS_ERROR;
    } else{
        parser->currentToken = parser->lineTokens[parser->tokenIdx++];
    }
    parser->diag->popTrace(parser->diag);
}

static void parserAdvanceExpect(luccix_assembler_parser* parser, luccix_assembler_token_type type){
    parser->diag->addTrace(parser->diag, __func__);
    if(parser->currentToken->type != type){
        parser->diag->print(parser->diag, DIAG_LEVEL_ERROR, "Expected `%s` but got `%s`\n", tokenTypeToCstr(type), parser->currentToken->data);
        parser->status = PARSER_STATUS_ERROR;
    } else{
        parserAdvance(parser);
    }
    parser->diag->popTrace(parser->diag);
} 

static void parserNextLine(luccix_assembler_parser* parser){
    parser->diag->addTrace(parser->diag, __func__);
    util_da_free(parser->lineTokens);
    parser->lineTokens = parser->lexer->lexLine(parser->lexer);
    parser->tokenIdx = 0;
    parserAdvance(parser);
    parser->diag->popTrace(parser->diag);
}


static luccix_assembler_ast_node* parseLabelDecl(luccix_assembler_parser* this){
    this->diag->addTrace(this->diag, __func__);
    luccix_assembler_ast_node* node = createAstNode(AST_NODE_LABEL_DECL);
    luccix_assembler_ast_label_decl_type declType;
    switch(this->currentToken->type){
        case TOKEN_TYPE_KEYWORD_GLOBAL: {
            declType = LABEL_DECL_GLOBAL;
            parserAdvanceExpect(this, TOKEN_TYPE_KEYWORD_GLOBAL);
        } break;
        case TOKEN_TYPE_INVALID:
        default: {
            this->diag->print(this->diag, DIAG_LEVEL_ICE, "Parser mistook `%s` for `global`\n", this->currentToken->data);
            this->status = PARSER_STATUS_ERROR;
            return NULL;
        } break;
    }
    node->label_decl.name = this->currentToken->data;
    node->label_decl.type = declType;
    parserAdvanceExpect(this, TOKEN_TYPE_IDENTIFIER);
    this->diag->popTrace(this->diag);
    return node;
}

static luccix_assembler_ast* parseAst(luccix_assembler_parser* this){
    luccix_assembler_ast* ast = createAst();
    while(this->currentToken->type != TOKEN_TYPE_EOF && this->status != PARSER_STATUS_ERROR){
        parserNextLine(this);
        for(int64_t i = 0; i < util_da_count(this->lineTokens); ++i){
            printf("Token %ld = %s\n", i, this->lineTokens[i]->data);
        }
        for(int64_t i = 0; i < util_da_count(this->lineTokens) && this->status != PARSER_STATUS_ERROR; ++i){
            luccix_assembler_ast_node* node = createAstNode(AST_NODE_INVALID);
            switch(this->currentToken->type){
                case TOKEN_TYPE_KEYWORD_GLOBAL: {
                    node = parseLabelDecl(this);
                } break;
                case TOKEN_TYPE_INVALID:
                default: {
                    this->diag->print(this->diag, DIAG_LEVEL_ERROR, "Invalid token `%s`\n", this->currentToken->data);
                    this->status = PARSER_STATUS_ERROR;
                } break;
            }
            if(node == NULL || node->type == AST_NODE_INVALID){
                this->diag->print(this->diag, DIAG_LEVEL_ICE, "Parser failed to proprely parse line\nNode was NULL or invalid\n");
                this->status = PARSER_STATUS_ERROR;
            }
        }
    }
    return ast;
}

luccix_assembler_parser* parserFromLexer(luccix_assembler_lexer* lexer, luccix_diagnostic* diag){
    luccix_assembler_parser* parser = malloc(sizeof(parser[0]));
    parser->lexer = lexer;
    parser->diag = diag;
    parser->parseToAst = parseAst;
    return parser;
}

void destroyParser(luccix_assembler_parser* this){
    if(util_da_capacity(this->lineTokens) > 0){
        util_da_free(this->lineTokens);
    }
    free(this);
}