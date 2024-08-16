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
#include <csignal>
using namespace luccix::assembler;

Context* context = nullptr;

void sigsegvHandler(int code){
    context->diag->print(DiagLevel::Ice, "Sigsegv %d\n", code);
    context->diag->printTrace();
    std::exit(1);
}

int real_main(int argc, char** argv);

int main(int argc, char** argv){
    try{
        real_main(argc, argv);
    } catch(std::exception& e){
        if(context != nullptr && context->diag != nullptr){
            context->diag->printTrace();
            context->diag->print(DiagLevel::Ice, "Exception: `%s`\n", e.what());
        } else{
            std::printf("Excpetion occured: `%s`\n", e.what());
        }
    }
}

int real_main(int argc, char** argv){
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
    signal(SIGSEGV, sigsegvHandler);
    context = new Context(file_contents, file_path, out_file, verbose, useColors);
    IrTree* tree = context->semagen->getTree();
    delete tree;
    delete context;
    return 0;
}