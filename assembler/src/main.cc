#ifdef COMPILE
#include <clopts.h>

using namespace command_line_options;

using options = clopts<
    positional<"file", "The file whose contents should be compiled", file<>, /*required=*/true>,
    flag<"-v", "Enable verbose printing">,
    // multOpt<"-f", "Nothing", std::string, {"orion", "elf", "bin"}>,
    help<>
>;
#endif

#include <context.h>
using namespace luccix::assembler;

int main(int argc, char** argv){
    std::string file_contents, file_path;
#ifdef COMPILE
    auto opts = options::parse(argc, argv);
    file_contents = opts.get<"file">()->contents;
    file_path = opts.get<"file">()->path;
#endif
    Context* context = new Context(file_contents, file_path, "./a.out", true, true);
    Lexer* lexer = context->lexer;
    std::vector<Token*> tokens = lexer->lex();
    for(Token* token : tokens){
        context->diag->print(token->getLoc(), DiagLevel::Note, "Token data = `%s` Token type = `%s`\n", token->getData().c_str(), tokenTypeToString(token->getType()).c_str());
    }
    // util_da_free(args->inputFiles);
    // free(args);
    delete context;
    return 0;
}