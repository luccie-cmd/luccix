#pragma once

typedef enum luccix_assembler_token_type{
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_EOF,
    // single tokens
    TOKEN_TYPE_COLON = ':',
    TOKEN_TYPE_COMMA = ',',
    // keywords
    TOKEN_TYPE_IDENTIFIER=255,
    TOKEN_TYPE_NUMBER,
} luccix_assembler_token_type;

typedef struct luccix_assembler_token{
    luccix_assembler_token_type type;
    const char* data;
} luccix_assembler_token;

luccix_assembler_token* createToken(luccix_assembler_token_type type, const char* data);
void tokenDestroy(luccix_assembler_token* this);