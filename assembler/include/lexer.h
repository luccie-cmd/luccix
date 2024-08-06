#pragma once
#include "diag.h"

typedef struct luccix_assembler_lexer{
    const char* inData;
    char c;
    size_t idx;
    luccix_diagnostic* diag;
} luccix_assembler_lexer;

luccix_assembler_lexer* lexer_from_file(const char* inFile, luccix_diagnostic* diag);