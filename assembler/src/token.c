#include <token.h>
#include <stdlib.h>
#include <util.h>

luccix_assembler_token* createToken(luccix_assembler_token_type type, const char* data){
    luccix_assembler_token* token = malloc(sizeof(token[0]));
    token->data = data;
    token->type = type;
    return token;
}
void tokenDestroy(luccix_assembler_token* this){
    luccix_assembler_list_free(this->data);
    free(this);
}