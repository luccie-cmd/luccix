#include <token.h>
#include <vector>
#include <utility>

namespace luccix::assembler{
std::vector<std::pair<std::string, TokenType>> pairs = {
    {"global", TokenType::KeywordGlobal},
    {"mov", TokenType::InstMov},
    {"syscall", TokenType::InstSyscall},
};

static TokenType dataToInstOrKeyword(std::string data){
    for(auto pair : pairs){
        if(pair.first == data){
            return pair.second;
        }
    }
    return TokenType::Identifier;
}

TokenType Token::getType(){
    return this->type;
}
std::string Token::getData(){
    return this->data;
}
Location* Token::getLoc(){
    return this->loc;
}

std::string tokenTypeToString(TokenType type){
    for(auto pair : pairs){
        if(pair.second == type){
            return pair.first;
        }
    }
    switch(type){
        case TokenType::Colon: 
        case TokenType::Comma: {
            return std::string(1, (char)type);
        } break;
        case TokenType::Eof: {
            return "Eof";
        } break;
        case TokenType::Eol: {
            return "Eol";
        } break;
        case TokenType::LiteralNumber: {
            return "Number";
        } break;
        case TokenType::Identifier: {
            return "Identifier";
        } break;
        case TokenType::Invalid: 
        default: {
            return "Invalid";
        } break;
    }
    return "Unreachable";
}

Token::Token(Location* pLoc, TokenType pType, std::string pData){
    this->data = pData;
    this->loc = pLoc;
    if(pType == TokenType::Identifier){
        this->type = dataToInstOrKeyword(this->data);
    } else{
        this->type = pType;
    }
}

Token::~Token(){}
}