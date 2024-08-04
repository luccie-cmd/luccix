#include <stdio.h>
#include <args.h>
#include <assert.h>

int main(int argc, char** argv){
    luccix_args *args = parse_args(argc, argv);
    assert(args != NULL);
    if(args->verbose){
        print_args(args);
    }
    return 0;
}