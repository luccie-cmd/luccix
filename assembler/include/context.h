#pragma once
#include "lexer.h"
#include "args.h"
#include "diag.h"

typedef struct luccix_assembler_context{
    const char* inputFile;
    const char* outputFile;
    luccix_assembler_lexer* lexer;
    luccix_diagnostic* diag;
} luccix_assembler_context;

luccix_assembler_context* context_from_args(luccix_assembler_args* args);
void context_destroy(luccix_assembler_context* context);