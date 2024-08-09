#include <context.h>
#include <stdlib.h>

luccix_assembler_context* context_from_args(luccix_assembler_args* args){
    luccix_assembler_context* context = malloc(sizeof(context[0]));
    context->inputFile = args->inputFile;
    context->outputFile = args->outFile;
    context->diag = diagnostic_from_args(args->useColor, args->verbose);
    context->lexer = lexer_from_file(context->inputFile, context->diag);
    context->parser = parserFromLexer(context->lexer, context->diag);
    return context;
}

void context_destroy(luccix_assembler_context* context){
    destroyParser(context->parser);
    lexer_destroy(context->lexer);
    free(context->diag);
    free(context);
}