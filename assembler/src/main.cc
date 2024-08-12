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
    context = new Context(file_contents, file_path, out_file, verbose, useColors);
    signal(SIGSEGV, sigsegvHandler);
    SyntaxTree* tree = context->parser->parseTree();
    for(SyntaxNode* node : tree->getNodes()){
        std::printf("Node type = %d\n", (int)node->getType());
        if(node->getType() == SyntaxNodeType::LabelDecl){
            auto decl = static_cast<SyntaxNodeLabelDecl*>(node);
            std::printf("Token start = `%s`\nToken name  = `%s`\n", decl->getStartKeyword()->getData().c_str(), decl->getName()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::Label){
            auto label = static_cast<SyntaxNodeLabel*>(node);
            std::printf("Token name = `%s`\n", label->getName()->getData().c_str());   
        } else if(node->getType() == SyntaxNodeType::Inst){
            auto inst = static_cast<SyntaxNodeInst*>(node);
            std::printf("Inst type = %d\n", (int)inst->getInstType());
        }
    }
    delete tree;
    delete context;
    return 0;
}