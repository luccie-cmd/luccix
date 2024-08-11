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
    Context* context = new Context("global test\ntest:", "./tests/assembler/test.asm", "./a.out", true, true);
    Lexer* lexer = context->lexer;
    std::vector<Token*> tokens = lexer->lexLine();
    for(Token* token : tokens){
        context->diag->print(DiagLevel::Note, "Token data = `%s`\n", token->getData().c_str());
    }
    // util_da_free(args->inputFiles);
    // free(args);
    delete context;
    return 0;
}