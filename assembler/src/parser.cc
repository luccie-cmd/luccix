#include "parser.h"
#include <algorithm>
#include <cstring>
#include <expat_config.h>

std::vector<luccix::assembler::RegisterInfo> possibleRegisters = {
    {"al", luccix::assembler::RegisterType::Byte}, {"bl", luccix::assembler::RegisterType::Byte}, {"cl", luccix::assembler::RegisterType::Byte}, {"dl", luccix::assembler::RegisterType::Byte},
    {"ah", luccix::assembler::RegisterType::Byte}, {"bh", luccix::assembler::RegisterType::Byte}, {"ch", luccix::assembler::RegisterType::Byte}, {"dh", luccix::assembler::RegisterType::Byte},
    {"sil", luccix::assembler::RegisterType::Byte}, {"dil", luccix::assembler::RegisterType::Byte}, {"bpl", luccix::assembler::RegisterType::Byte}, {"spl", luccix::assembler::RegisterType::Byte},
    {"r8b", luccix::assembler::RegisterType::Byte}, {"r9b", luccix::assembler::RegisterType::Byte}, {"r10b", luccix::assembler::RegisterType::Byte}, {"r11b", luccix::assembler::RegisterType::Byte},
    {"r12b", luccix::assembler::RegisterType::Byte}, {"r13b", luccix::assembler::RegisterType::Byte}, {"r14b", luccix::assembler::RegisterType::Byte}, {"r15b", luccix::assembler::RegisterType::Byte},
    {"ax", luccix::assembler::RegisterType::Word}, {"bx", luccix::assembler::RegisterType::Word}, {"cx", luccix::assembler::RegisterType::Word}, {"dx", luccix::assembler::RegisterType::Word},
    {"si", luccix::assembler::RegisterType::Word}, {"di", luccix::assembler::RegisterType::Word}, {"bp", luccix::assembler::RegisterType::Word}, {"sp", luccix::assembler::RegisterType::Word},
    {"r8w", luccix::assembler::RegisterType::Word}, {"r9w", luccix::assembler::RegisterType::Word}, {"r10w", luccix::assembler::RegisterType::Word}, {"r11w", luccix::assembler::RegisterType::Word},
    {"r12w", luccix::assembler::RegisterType::Word}, {"r13w", luccix::assembler::RegisterType::Word}, {"r14w", luccix::assembler::RegisterType::Word}, {"r15w", luccix::assembler::RegisterType::Word},
    {"eax", luccix::assembler::RegisterType::Dword}, {"ebx", luccix::assembler::RegisterType::Dword}, {"ecx", luccix::assembler::RegisterType::Dword}, {"edx", luccix::assembler::RegisterType::Dword},
    {"esi", luccix::assembler::RegisterType::Dword}, {"edi", luccix::assembler::RegisterType::Dword}, {"ebp", luccix::assembler::RegisterType::Dword}, {"esp", luccix::assembler::RegisterType::Dword},
    {"r8d", luccix::assembler::RegisterType::Dword}, {"r9d", luccix::assembler::RegisterType::Dword}, {"r10d", luccix::assembler::RegisterType::Dword}, {"r11d", luccix::assembler::RegisterType::Dword},
    {"r12d", luccix::assembler::RegisterType::Dword}, {"r13d", luccix::assembler::RegisterType::Dword}, {"r14d", luccix::assembler::RegisterType::Dword}, {"r15d", luccix::assembler::RegisterType::Dword},
    {"rax", luccix::assembler::RegisterType::Qword}, {"rbx", luccix::assembler::RegisterType::Qword}, {"rcx", luccix::assembler::RegisterType::Qword}, {"rdx", luccix::assembler::RegisterType::Qword},
    {"rsi", luccix::assembler::RegisterType::Qword}, {"rdi", luccix::assembler::RegisterType::Qword}, {"rbp", luccix::assembler::RegisterType::Qword}, {"rsp", luccix::assembler::RegisterType::Qword},
    {"r8", luccix::assembler::RegisterType::Qword}, {"r9", luccix::assembler::RegisterType::Qword}, {"r10", luccix::assembler::RegisterType::Qword}, {"r11", luccix::assembler::RegisterType::Qword},
    {"r12", luccix::assembler::RegisterType::Qword}, {"r13", luccix::assembler::RegisterType::Qword}, {"r14", luccix::assembler::RegisterType::Qword}, {"r15", luccix::assembler::RegisterType::Qword},
    {"xmm0", luccix::assembler::RegisterType::XMM}, {"xmm1", luccix::assembler::RegisterType::XMM}, {"xmm2", luccix::assembler::RegisterType::XMM}, {"xmm3", luccix::assembler::RegisterType::XMM},
    {"xmm4", luccix::assembler::RegisterType::XMM}, {"xmm5", luccix::assembler::RegisterType::XMM}, {"xmm6", luccix::assembler::RegisterType::XMM}, {"xmm7", luccix::assembler::RegisterType::XMM},
    {"ymm0", luccix::assembler::RegisterType::YMM}, {"ymm1", luccix::assembler::RegisterType::YMM}, {"ymm2", luccix::assembler::RegisterType::YMM}, {"ymm3", luccix::assembler::RegisterType::YMM},
    {"ymm4", luccix::assembler::RegisterType::YMM}, {"ymm5", luccix::assembler::RegisterType::YMM}, {"ymm6", luccix::assembler::RegisterType::YMM}, {"ymm7", luccix::assembler::RegisterType::YMM},
    {"zmm0", luccix::assembler::RegisterType::ZMM}, {"zmm1", luccix::assembler::RegisterType::ZMM}, {"zmm2", luccix::assembler::RegisterType::ZMM}, {"zmm3", luccix::assembler::RegisterType::ZMM},
    {"zmm4", luccix::assembler::RegisterType::ZMM}, {"zmm5", luccix::assembler::RegisterType::ZMM}, {"zmm6", luccix::assembler::RegisterType::ZMM}, {"zmm7", luccix::assembler::RegisterType::ZMM}
};


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
        } else if(this->idx >= this->lexer->getCachedTokens().size()){
            this->currentToken = new Token(this->currentToken->getLoc(), TokenType::Eof, "Eof");
        } else{
            this->currentToken = this->lineTokens.at(this->idx++);
        }
        // this->diag->print(retToken->getLoc(), DiagLevel::Note, "IDX = %ld, Linetokens = %ld, Total tokens = %ld\n", this->idx, this->lineTokens.size(),this->lexer->getCachedTokens().size());
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
        }
        this->diag->popTrace();
        return retToken;
    }
    Token* Parser::tryConsume(TokenType type, std::string error){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* retToken = this->consume();
        if(retToken->getType() != type){
            this->diag->print(retToken->getLoc(), DiagLevel::Error, "Expected %s but got `%s`\n", error.c_str(), tokenTypeToString(retToken->getType()).c_str());
            this->diag->printVerbose("Token data = `%s`\n", retToken->getData().c_str());
            this->status = ParserStatus::Error;
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
        // TODO: Support NASM like names
        this->tryConsume(TokenType::Colon, "':'");
        this->tryConsume(TokenType::Eol, "'Newline'");
        this->diag->popTrace();
        return new SyntaxNodeLabel(nameToken);
    }
    SyntaxNode* Parser::parseInst(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* instToken = this->consume();
        std::vector<SyntaxNode*> args;
        while(this->currentToken->getType() != TokenType::Eol){
            args.push_back(this->parseNode());
            if(this->currentToken->getType() != TokenType::Eol){
                this->tryConsume(TokenType::Comma, "','");
            }
        }
        this->tryConsume(TokenType::Eol, "'Newline'");
        this->diag->popTrace();
        return new SyntaxNodeInst(instToken, args);
    }
    SyntaxNode* Parser::parseNameref(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* namerefToken = this->consume();
        this->diag->popTrace();
        return new SyntaxNodeNameref(namerefToken);
    }
    SyntaxNode* Parser::parseLiteralNumber(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Token* numberToken = this->consume();
        this->diag->popTrace();
        return new SyntaxNodeLiteralNumber(numberToken);
    }
    SyntaxNode* Parser::parseNode(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        Location* beginLoc = new Location(*this->currentToken->getLoc());
        SyntaxNode* node = nullptr;
        switch(this->currentToken->getType()){
            case TokenType::KeywordGlobal: {
                node = this->parseLabelDecl();
            } break;
            case TokenType::Identifier: {
                if(this->peekToken(0) != nullptr && this->peekToken(0)->getType() == TokenType::Colon){
                    node = this->parseLabel();
                } else if (auto it = std::find_if(possibleRegisters.begin(), possibleRegisters.end(),
                           [this](const RegisterInfo& reg) {
                               return std::strcmp(reg.name, this->currentToken->getData().c_str()) == 0;
                           }); it != possibleRegisters.end()) {
                    Token* registerToken = this->tryConsume(TokenType::Identifier, "ICE: Invalid register identifier");
                    node = new SyntaxNodeRegister(registerToken, *it.base());
                } else{
                    node = this->parseNameref();
                }
            } break;
            case TokenType::LiteralNumber: {
                node = this->parseLiteralNumber();
            } break;

            case TokenType::InstSyscall:
            case TokenType::InstRet:
            case TokenType::InstMov: {
                node = this->parseInst();
            } break;

            case TokenType::Eol: {
                this->consume();
                node = this->parseLine();
            } break;

            case TokenType::Eof: {
                this->consume();
                this->status = ParserStatus::Done;
                return nullptr;
            } break;

            default: {
                this->diag->print(beginLoc, DiagLevel::Error, "Invalid token\n");
                this->diag->print(DiagLevel::Note, "current token data = `%s`\n", this->currentToken->getData().c_str());
                this->consume();
                this->status = ParserStatus::Error;
            } break;
        }
        this->diag->popTrace();
        return node;
    }
    SyntaxNode* Parser::parseLine(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        this->lineTokens = this->lexer->lexLine();
        this->idx = 0;
        (void)this->consume(); // will always be a nullptr
        Location* beginLoc = new Location(*this->currentToken->getLoc());
        SyntaxNode* node = this->parseNode();
        if(this->status != ParserStatus::Done && node == nullptr){
            this->diag->printTrace();
            this->diag->print(beginLoc, DiagLevel::Ice, "Node is or returned NULL\n");
            this->diag->print(DiagLevel::Note, "current data = `%s`\n", this->currentToken->getData().c_str());
            this->diag->print(DiagLevel::Note, "All line tokens:\n");
            for(Token* token : this->lineTokens){
                this->diag->print(token->getLoc(), DiagLevel::Note, "data = `%s`\n", token->getData().c_str());
            }
            this->status = ParserStatus::Error;
        }
        this->diag->popTrace();
        return node;
    }
    SyntaxTree* Parser::parseTree(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        SyntaxTree* tree = new SyntaxTree;
        while(this->status != ParserStatus::Done && this->status != ParserStatus::Error){
            SyntaxNode* node = this->parseLine();
            if(node == nullptr){
                break;
            }
            tree->pushNode(node);
        }
        if(this->status == ParserStatus::Error){
            this->diag->printTrace();
            this->diag->print(DiagLevel::Error, "Parser error\n");
        }
        this->diag->print(DiagLevel::Note, "Parsed Tokens\n");
        this->diag->popTrace();
        return tree;
    }
};