#pragma once

typedef enum luccix_assembler_token_type{
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_EOF,
    // single tokens
    TOKEN_TYPE_COLON = ':',
    TOKEN_TYPE_COMMA = ',',
    // keywords
    __TOKEN_TYPE_KEYWORD_START=255,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_KEYWORD_GLOBAL,

    __TOKEN_TYPE_INST_START=300,
    TOKEN_TYPE_INST_MOV,
    TOKEN_TYPE_INST_SYSCALL,
} luccix_assembler_token_type;

typedef struct luccix_assembler_token{
    luccix_assembler_token_type type;
    const char* data;
} luccix_assembler_token;

luccix_assembler_token* createToken(luccix_assembler_token_type type, const char* data);
void tokenDestroy(luccix_assembler_token* this);
const char* tokenTypeToCstr(luccix_assembler_token_type type);