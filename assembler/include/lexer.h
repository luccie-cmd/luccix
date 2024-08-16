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
        std::vector<Token*> cachedTokens;
        Token* lexIdentifier();
        Token* lexNumber();
        void advance();
        void skipWhitespace(bool skipNewline);
        std::vector<Token*> lex();
    public:
        Lexer(std::string data, std::string inFileName, Diag* diag);
        ~Lexer();
        std::vector<Token*> getCachedTokens();
        std::vector<Token*> lexLine();
};
}