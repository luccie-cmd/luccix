#pragma once
#include <string>
#include "diag.h"

namespace luccix::assembler{
    enum struct TokenType : int {
        Invalid,
        Eof,
        Eol, // End of line
        // single tokens
        Colon = ':',
        Comma = ',',
        // keywords
        __KEYWORD_START=255,
        Identifier,
        LiteralNumber,
        KeywordGlobal,

        __INST_START=300,
        InstMov,
        InstSyscall,
    };

    std::string tokenTypeToString(TokenType type);

    class Token{
        private:
            TokenType type;
            std::string data;
            Location* loc;
        public:
            Token(Location* loc, TokenType type, std::string data);
            TokenType getType();
            std::string getData();
            Location* getLoc();
            ~Token();
    };
}