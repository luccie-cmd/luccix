#ifdef COMPILE
#include <clopts.h>

using namespace command_line_options;

using options = clopts<
    positional<"file", "The file whose contents should be compiled", file<>, /*required=*/true>,
    flag<"-v", "Enable verbose printing">,
    option<"-o", "Outfile file", std::string>,
    option<"--color", "Use colors", values<"always", "never", "default">>,
    option<"-f", "Output format", values<"ori", "bin">>,
    help<>
>;
#endif

#include <context.h>
using namespace luccix::assembler;

int main(int argc, char** argv){
    std::string file_contents, file_path, out_file;
    bool verbose, useColors;
#ifdef COMPILE
    auto opts = options::parse(argc, argv);
    file_contents = opts.get<"file">()->contents;
    file_path = opts.get<"file">()->path;
    out_file = opts.get_or<"-o">("a.out");
    verbose = opts.get<"-v">();
    std::string colorOpt = opts.get_or<"--color">("always");
    useColors = colorOpt == "always";
#endif
    Context* context = new Context(file_contents, file_path, out_file, verbose, useColors);
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