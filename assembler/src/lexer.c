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
    if(this->idx > strlen(this->inData)){
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
    luccix_assembler_list(char) data;
    luccix_assembler_list_init(data);
    luccix_assembler_list_push(data, this->c);
    lexerAdvance(this);
    while(isdigit(this->c)){
        luccix_assembler_list_push(data, this->c);
        lexerAdvance(this);
    }
    return createToken(TOKEN_TYPE_NUMBER, (const char*)data);
}

static luccix_assembler_token* lexIdentifier(luccix_assembler_lexer* this){
    luccix_assembler_list(char) data;
    luccix_assembler_list_init(data);
    luccix_assembler_list_push(data, this->c);
    lexerAdvance(this);
    while(isContinueIdentifier(this->c)){
        luccix_assembler_list_push(data, this->c);
        lexerAdvance(this);
    }
    return createToken(TOKEN_TYPE_IDENTIFIER, (const char*)data);
}

static luccix_assembler_list(luccix_assembler_token*)lexLine(luccix_assembler_lexer* this){
    lexerSkipWhitespace(this);
    luccix_assembler_list(luccix_assembler_token*) line;
    luccix_assembler_list_init(line);
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
            case '_': {
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
            return line;
        }
        luccix_assembler_list_push(line, token);
    }
    lexerAdvance(this);
    return line;
}

luccix_assembler_lexer* lexer_from_file(const char* inFile, luccix_diagnostic* diag){
    luccix_assembler_lexer* lexer = malloc(sizeof(lexer[0]));
    lexer->inData = readFile(inFile);
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