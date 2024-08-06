#include <lexer.h>
#include <stdlib.h>
#include <util.h>

luccix_assembler_lexer* lexer_from_file(const char* inFile, luccix_diagnostic* diag){
    luccix_assembler_lexer* lexer = malloc(sizeof(lexer[0]));
    lexer->inData = readFile(inFile);
    lexer->diag = diag;
    lexer->diag->printVerbose(lexer->diag, "Lexer data:\n`%s`\n", lexer->inData);
    return lexer;
}