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
    free(args);
    free(context);
    return 0;
}