#include <stdio.h>
#include <args.h>
#include <context.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv){
    luccix_assembler_args *args = parse_args(argc, argv);
    assert(args != NULL);
    if(args->verbose){
        print_args(args);
    }
    luccix_assembler_context* context = context_from_args(args);
    util_da_free(args->inputFiles);
    free(args);
    luccix_assembler_parser* parser = context->parser;
    // luccix_assembler_ast* ast = parser->parseToAst(parser);
    // if(parser->status == PARSER_STATUS_ERROR){
    //     context->diag->print(context->diag, DIAG_LEVEL_ERROR, "Parser error\n");
    //     return 1;
    // }
    // printAst(ast);
    (void)parser;
    context_destroy(context);
    return 0;
}