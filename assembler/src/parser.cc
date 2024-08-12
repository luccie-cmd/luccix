#include "parser.h"

namespace luccix::assembler{
    Parser::Parser(Lexer* lexer, Diag* diag){
        this->lexer = lexer;
        this->diag = diag;
        this->status = ParserStatus::Progress;
        this->currentToken = nullptr;
    }
    Parser::~Parser(){}
    Token* Parser::consume(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* retToken = nullptr;
        if(this->currentToken != nullptr) {
            retToken = new Token(*this->currentToken);
        }
        if(this->idx >= this->lineTokens.size()){
            this->currentToken = new Token(this->currentToken->getLoc(), TokenType::Eol, "Eol");
        } else{
            this->currentToken = this->lineTokens.at(this->idx++);
        }
        this->diag->popTrace();
        return retToken;
    }
    Token* Parser::peekToken(std::size_t ahead){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* retToken = nullptr;
        try{
            retToken = this->lineTokens.at(this->idx+ahead);
        } catch(std::exception& e){
            this->diag->printTrace();
            this->diag->print(this->currentToken != nullptr ? this->currentToken->getLoc() : this->lineTokens.at(0)->getLoc(), DiagLevel::Ice, "Expected a token, but got an end of list\n");
            this->status = ParserStatus::Error;
            std::exit(1);
        }
        this->diag->popTrace();
        return retToken;
    }
    Token* Parser::tryConsume(TokenType type, std::string error){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* retToken = this->consume();
        if(retToken->getType() != type){
            this->status = ParserStatus::Error;
            this->diag->print(retToken->getLoc(), DiagLevel::Error, "Expected %s but got `%s`\nGotten type = `%s`\n", error.c_str(), tokenTypeToString(type).c_str(), tokenTypeToString(retToken->getType()).c_str());
        }
        this->diag->popTrace();
        return retToken;
    }
    SyntaxNode* Parser::parseLabelDecl(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* typeToken = this->consume();
        Token* nameToken = this->tryConsume(TokenType::Identifier, "'Identifier'");
        this->tryConsume(TokenType::Eol, "'Newline'");
        this->diag->popTrace();
        return new SyntaxNodeLabelDecl(typeToken, nameToken);
    }
    SyntaxNode* Parser::parseLabel(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* nameToken = this->consume();
        this->tryConsume(TokenType::Colon, "':'");
        this->tryConsume(TokenType::Eol, "'Newline'");
        this->diag->popTrace();
        return new SyntaxNodeLabel(nameToken);
    }
    SyntaxNode* Parser::parseLine(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        this->lineTokens = this->lexer->lexLine();
        this->idx = 0;
        (void)this->consume(); // will always be a nullptr
        Location* beginLoc = new Location(*this->currentToken->getLoc());
        SyntaxNode* node = nullptr;
        switch(this->currentToken->getType()){
            case TokenType::KeywordGlobal: {
                node = this->parseLabelDecl();
            } break;
            case TokenType::Identifier: {
                if(this->peekToken(0) != nullptr && this->peekToken(0)->getType() == TokenType::Colon){
                    node = this->parseLabel();
                } else{
                    goto Invalid;
                }
            } break;

            Invalid:;
            default: {
                this->diag->print(beginLoc, DiagLevel::Error, "Invalid token\n");
                this->diag->print(DiagLevel::Note, "current token data = `%s`\n", this->currentToken->getData().c_str());
                this->status = ParserStatus::Error;
            } break;
        }
        if(node == nullptr){
            this->diag->printTrace();
            this->diag->print(beginLoc, DiagLevel::Ice, "Node is or returned NULL\n");
            this->diag->print(DiagLevel::Note, "current data = `%s`\n", this->currentToken->getData().c_str());
            this->diag->print(DiagLevel::Note, "All line tokens:\n");
            for(Token* token : this->lineTokens){
                this->diag->print(token->getLoc(), DiagLevel::Note, "data = `%s`\n", token->getData().c_str());
            }
            this->status = ParserStatus::Error;
            std::exit(1);
        }
        this->diag->popTrace();
        return node;
    }
};