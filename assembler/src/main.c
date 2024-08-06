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
    luccix_assembler_list_free(args->inputFiles);
    free(args);
    luccix_assembler_list(luccix_assembler_token*) line = context->lexer->lexLine(context->lexer);
    while(line != NULL && luccix_assembler_list_len(line) != 0 && context->lexer->status == LEXER_STATUS_DONE){
        // printf("Line = `%s`\n", line);
        for(size_t i = 0; i < luccix_assembler_list_len(line); ++i){
            printf("Token data = `%s` Token type = %s\n", luccix_assembler_list_idx(line, i)->data, tokenTypeToCstr(luccix_assembler_list_idx(line, i)->type));
        }
        luccix_assembler_list_free(line);
        line = context->lexer->lexLine(context->lexer);
    }
    luccix_assembler_list_free(line);
    context_destroy(context);
    return 0;
}