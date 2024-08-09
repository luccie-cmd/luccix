#pragma once
#include "ast.h"
#include "lexer.h"
#include "diag.h"

typedef enum luccix_assembler_parser_status{
    PARSER_STATUS_INVALID,
    PARSER_STATUS_DONE,
    PARSER_STATUS_ERROR,
} luccix_assembler_parser_status;

typedef struct luccix_assembler_parser{
    luccix_assembler_parser_status status;
    luccix_assembler_lexer* lexer;
    luccix_assembler_token* currentLabelToken;
    luccix_assembler_token* currentToken;
    int64_t tokenIdx;
    util_da(luccix_assembler_token*) lineTokens;
    luccix_diagnostic* diag;
    luccix_assembler_ast* (*parseToAst)(struct luccix_assembler_parser* this);
} luccix_assembler_parser;

luccix_assembler_parser* parserFromLexer(luccix_assembler_lexer* lexer, luccix_diagnostic* diag);
void destroyParser(luccix_assembler_parser* this);