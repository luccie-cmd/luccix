#pragma once
#include <string>
#include "lexer.h"
#include "args.h"
#include "diag.h"

namespace luccix::assembler{
class Context{
    private:
        std::string outputFile;
        Lexer* lexer;
        Diag* diag;
    public:
        Context(std::string inData, std::string outDir, bool verbose, bool useColors);
        ~Context();
};
}