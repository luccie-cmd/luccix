#pragma once
#include <string>
#include "lexer.h"
#include "diag.h"
#include "parser.h"

namespace luccix::assembler{
class Context{
    private:
        std::string outputFile;
        Diag* diag;
        Lexer* lexer;
    public:
        Parser* parser;
        Context(std::string inData, std::string inFileName, std::string outDir, bool verbose, bool useColors);
        ~Context();
};
}