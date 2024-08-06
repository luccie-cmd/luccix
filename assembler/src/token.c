#include <token.h>
#include <stdlib.h>
#include <util.h>
#include <string.h>

typedef struct tokenDataTokenTypePair{
    const char* data;
    luccix_assembler_token_type type;
} tokenDataTokenTypePair;
tokenDataTokenTypePair pairs[] = {
    {"global", TOKEN_TYPE_KEYWORD_GLOBAL},
    {"mov", TOKEN_TYPE_INST_MOV},
    {"syscall", TOKEN_TYPE_INST_SYSCALL},
};

static luccix_assembler_token_type dataToInstOrKeyword(const char* data){
    for(size_t i = 0; i < ARRAY_SIZE(pairs); ++i){
        if(strcmp(pairs[i].data, data) == 0){
            return pairs[i].type;
        }
    }
    return TOKEN_TYPE_IDENTIFIER;
}

char buffer[2];

const char* tokenTypeToCstr(luccix_assembler_token_type type){
    for(size_t i = 0; i < ARRAY_SIZE(pairs); ++i){
        if(pairs[i].type == type){
            return pairs[i].data;
        }
    }
    switch(type){
        case TOKEN_TYPE_COLON: 
        case TOKEN_TYPE_COMMA: {
            buffer[0] = (char)type;
            buffer[1] = '\0';
            return buffer;
        } break;
        case TOKEN_TYPE_EOF: {
            return "EOF";
        } break;
        case TOKEN_TYPE_NUMBER: {
            return "NUMBER";
        } break;
        case TOKEN_TYPE_IDENTIFIER: {
            return "IDENTIFIER";
        } break;
        case TOKEN_TYPE_INVALID:
        default: {
            return "Invalid";
        } break;
    }
    return "Unreachable";
}

luccix_assembler_token* createToken(luccix_assembler_token_type type, const char* data){
    luccix_assembler_token* token = malloc(sizeof(token[0]));
    token->data = data;
    if(type == TOKEN_TYPE_IDENTIFIER){
        token->type = dataToInstOrKeyword(data);
    } else{
        token->type = type;
    }
    return token;
}
void tokenDestroy(luccix_assembler_token* this){
    luccix_assembler_list_free(this->data);
    free(this);
}