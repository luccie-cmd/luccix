#include <context.h>
#include <cassert>

namespace luccix::assembler{
    static void printArgs(Diag* diag, std::string inFileName, std::string outFile, bool verbose, bool useColors){
        diag->printVerbose("Input file %s\n", inFileName.c_str());
        diag->printVerbose("Output file %s\n", outFile.c_str());
        diag->printVerbose("Verbose printing %s\n", verbose == true ? "Yes" : "No");
        diag->printVerbose("Use colors %s\n", useColors == true ? "Yes" : "No");
    }
    Context::Context(std::string inFileData, std::string inFileName, std::string outFile, bool verbose, bool useColors){
        this->outputFile = outFile;
        this->diag = new Diag(useColors, verbose);
        assert(this->diag != nullptr);
        printArgs(this->diag, inFileName, outFile, verbose, useColors);
        this->lexer = new Lexer(inFileData, inFileName, this->diag);
        this->parser = new Parser(this->lexer, this->diag);
        SyntaxTree* tree = this->parser->parseTree();
        this->semagen = new SemaGen(tree, this->diag);
        diag->printVerbose("TODO: Code gen\n");
    }
    Context::~Context(){
        delete semagen                  ;
        delete parser;
        delete lexer;
        delete diag;
    }
}