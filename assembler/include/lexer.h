#pragma once
#include "diag.h"
#include "util.h"
#include "token.h"
#include <stddef.h>

typedef struct {
    luccix_assembler_token** elements;
    int64_t count;
    int64_t capacity;
} luccix_assembler_token_list;

typedef enum luccix_assembler_lexer_status{
    LEXER_STATUS_INVALID,
    LEXER_STATUS_DONE,
    LEXER_STATUS_ERROR,
} luccix_assembler_lexer_status;

typedef struct luccix_assembler_lexer{
    const char* inData;
    char c;
    int64_t idx;
    luccix_diagnostic* diag;
    luccix_assembler_lexer_status status;
    util_da(luccix_assembler_token*) (*lexLine)(struct luccix_assembler_lexer* this);
} luccix_assembler_lexer;

luccix_assembler_lexer* lexer_from_file(const char* inFile, luccix_diagnostic* diag);
void lexer_destroy(luccix_assembler_lexer* lexer);