#pragma once
#include <string>

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
        public:
            Token(TokenType type, std::string data);
            ~Token();
    };
}