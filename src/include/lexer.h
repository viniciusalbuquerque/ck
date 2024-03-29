#ifndef CK_LEXER_H
#define CK_LEXER_H
#include <stdlib.h>
#include "token.h"

typedef struct LEXER_STRUCT {
    unsigned int pos;
    char current;
    char* text;
    size_t text_size;
} lexer_s;

lexer_s* lexer_init(char* text);
void lexer_next(lexer_s* lexer);
void lexer_previous(lexer_s* lexer);
token_s* lexer_next_token(lexer_s* lexer);
token_s*lexer_parse_number_token(lexer_s* lexer);
#endif
