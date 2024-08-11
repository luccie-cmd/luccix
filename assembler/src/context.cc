#include <context.h>

namespace luccix::assembler{
    Context::Context(std::string inFileData, std::string outFile, bool verbose, bool useColors){
        this->outputFile = outFile;
        this->diag = new Diag(useColors, verbose);
        this->lexer = new Lexer(inFileData, this->diag);
    }
    Context::~Context(){
        delete lexer;
        delete diag;
    }
}