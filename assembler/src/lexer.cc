#include <lexer.h>
#include <stdlib.h>
#include <util.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void lexerAdvance(luccix_assembler_lexer* this){
    if(this->c == '\0' && this->idx != 0){
        return;
    }
    if(this->idx > (int64_t)strlen(this->inData)){
        this->diag->print(this->diag, DIAG_LEVEL_ICE, "Check for EOF failed\nIDX = %ld max IDX = %ld\n", this->idx, strlen(this->inData));
        exit(1);
    }
    this->c = this->inData[this->idx++];
}

static void lexerSkipWhitespace(luccix_assembler_lexer* this){
    while(isspace(this->c) && this->c != '\n'){
        lexerAdvance(this);
    }
}

static int isContinueIdentifier(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

static luccix_assembler_token* lexNumber(luccix_assembler_lexer* this){
    util_string data = util_string_create(util_default_allocator);
    util_string_append_rune(&data, this->c);
    lexerAdvance(this);
    while(isdigit(this->c)){
        util_string_append_rune(&data, this->c);
        lexerAdvance(this);
    }
    return createToken(TOKEN_TYPE_NUMBER, util_string_as_cstring(data));
}

static luccix_assembler_token* lexIdentifier(luccix_assembler_lexer* this){
    util_string data = util_string_create(util_default_allocator);
    util_string_append_rune(&data, this->c);
    lexerAdvance(this);
    while(isContinueIdentifier(this->c)){
        util_string_append_rune(&data, this->c);
        lexerAdvance(this);
    }
    return createToken(TOKEN_TYPE_IDENTIFIER, util_string_as_cstring(data));
}

static util_da(luccix_assembler_token*) lexLine(luccix_assembler_lexer* this){
    lexerSkipWhitespace(this);
    util_da(luccix_assembler_token*) line;
    util_da_reserve(line, 1);
    while(this->c != '\n' && this->c != '\0' && this->status != LEXER_STATUS_ERROR){
        lexerSkipWhitespace(this);
        luccix_assembler_token* token = NULL;
        switch(this->c){
            case 'a': case 'b': case 'c': case 'd': case 'e':
            case 'f': case 'g': case 'h': case 'i': case 'j':
            case 'k': case 'l': case 'm': case 'n': case 'o':
            case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
            case 'A': case 'B': case 'C': case 'D': case 'E':
            case 'F': case 'G': case 'H': case 'I': case 'J':
            case 'K': case 'L': case 'M': case 'N': case 'O':
            case 'P': case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case '_': case '.': {
                token = lexIdentifier(this);
            } break;

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': {
                token = lexNumber(this);
            } break;

            case ':':
            case ',': {
                char buffer[2];
                buffer[0] = this->c;
                buffer[1] = '\0';
                token = createToken(this->c, (const char*)buffer);
                lexerAdvance(this);
            } break;

            default: {
                this->diag->print(this->diag, DIAG_LEVEL_ERROR, "Invalid character `%c`\n", this->c);
                this->status = LEXER_STATUS_ERROR;
            } break;
        }
        if(token == NULL){
        util_da_push(line, createToken(TOKEN_TYPE_EOF, "\0"));
            return line;
        }
        util_da_push(line, token);
    }
    lexerAdvance(this);
    if(this->c == '\0'){
        util_da_push(line, createToken(TOKEN_TYPE_EOF, "\0"));
    }
    return line;
}

luccix_assembler_lexer* lexer_from_file(const char* inFile, luccix_diagnostic* diag){
    luccix_assembler_lexer* lexer = malloc(sizeof(lexer[0]));
    lexer->inData = util_string_as_cstring(util_file_read(util_default_allocator, inFile));
    lexer->status = LEXER_STATUS_DONE;
    lexer->idx = 0;
    lexer->diag = diag;
    lexer->lexLine = lexLine;
    lexerAdvance(lexer);
    while(isspace(lexer->c)){
        lexerAdvance(lexer);
    }
    return lexer;
}

void lexer_destroy(luccix_assembler_lexer* lexer){
    free((void*)lexer->inData);
    free(lexer);
}