#include <token.h>

namespace luccix::assembler{
struct tokenDataTokenTypePair{
    std::string data;
    TokenType type;
} tokenDataTokenTypePair;

struct tokenDataTokenTypePair pairs[] = {
    {"global", TokenType::KeywordGlobal},
    {"mov", TokenType::InstMov},
    {"syscall", TokenType::InstSyscall},
};

static TokenType dataToInstOrKeyword(std::string data){
    for(uint64_t i = 0; i < sizeof(pairs)/sizeof(pairs[0]); ++i){
        if(data == pairs[i].data){
            return pairs[i].type;
        }
    }
    return TokenType::Identifier;
}

std::string tokenTypeToCstr(TokenType type){
    for(uint64_t i = 0; i < sizeof(pairs)/sizeof(pairs[0]); ++i){
        if(pairs[i].type == type){
            return pairs[i].data;
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

Token::Token(TokenType pType, std::string pData){
    this->data = pData;
    if(type == TokenType::Identifier){
        this->type = dataToInstOrKeyword(this->data);
    } else{
        this->type = pType;
    }
}

Token::~Token(){}
}