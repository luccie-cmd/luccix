#pragma once
#include <string>
#include "lexer.h"
#include "diag.h"
#include "parser.h"
#include "semagen.h"

namespace luccix::assembler{
class Context{
    private:
        std::string outputFile;
        Lexer* lexer;
        Parser* parser;
    public:
        SemaGen* semagen;
        Diag* diag; // sadly this is better to be a global var, because of the sigsegv handler
        Context(std::string inData, std::string inFileName, std::string outDir, bool verbose, bool useColors);
        ~Context();
};
}