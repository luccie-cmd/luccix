#pragma once
#include <string>
#include <vector>
#include "diag.h"
#include "token.h"

namespace luccix::assembler{
enum struct LexerStatus : int {
    Invalid,
    Done,
    Progress,
    Error,
};

class Lexer{
    private:
        std::string data;
        char c;
        std::size_t index;
        Diag* diag;
        LexerStatus status;
        Location *currentLocation;
        Token* lexIdentifier();
        Token* lexNumber();
        void advance();
        void skipWhitespace(); // will skip all whitespaces except for new lines (\n)
    public:
        Lexer(std::string data, std::string inFileName, Diag* diag);
        ~Lexer();
        std::vector<Token*> lex();
};
}