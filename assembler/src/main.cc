#include <args.h>
#include <context.h>

using namespace luccix::assembler;

int main(int argc, char** argv){
    (void)argc;
    (void)argv;
    // luccix_assembler_args *args = parse_args(argc, argv);
    // assert(args != NULL);
    // if(args->verbose){
    //     print_args(args);
    // }
    Context* context = new Context("global test\ntest:", "./a.out", 1, 1);
    // util_da_free(args->inputFiles);
    // free(args);
    delete context;
    return 0;
}