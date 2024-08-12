#pragma once
#include <string>
#include "lexer.h"
#include "diag.h"

namespace luccix::assembler{
class Context{
    private:
        std::string outputFile;
    public:
        Diag* diag;
        Lexer* lexer;
        Context(std::string inData, std::string inFileName, std::string outDir, bool verbose, bool useColors);
        ~Context();
};
}