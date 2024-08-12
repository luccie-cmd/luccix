#pragma once
#include <vector>
#include "token.h"
#include "lexer.h"
#include "ast.h"

namespace luccix::assembler{
    enum struct ParserStatus : int {
        Invalid,
        Done,
        Progress,
        Error,
    };
    class Parser{
        private:
            std::vector<Token*> lineTokens;
            std::size_t idx;
            Token* currentToken;
            Lexer* lexer;
            Diag* diag;
            ParserStatus status;
            Token* consume();
            // 0 is current 1 is after that, will return nullptr if at Eof or Eol
            Token* peekToken(std::size_t ahead);
            Token* tryConsume(TokenType type, std::string error);
            Token* tryConsume(std::string data, std::string error);
            SyntaxNode* parseLabelDecl();
            SyntaxNode* parseLabel();
        public:
            Parser(Lexer* lexer, Diag* diag);
            SyntaxNode* parseLine();
            // Ast* parseFile();
            ~Parser();
    };
};