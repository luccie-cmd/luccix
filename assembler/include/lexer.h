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
    public:
        Lexer(std::string data, Diag* diag);
        ~Lexer();
        std::vector<Token> lexLine();
};
}