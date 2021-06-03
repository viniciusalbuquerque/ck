#ifndef CK_PARSER_H
#define CK_PARSER_H
#include "lexer.h"
#include "token.h"

typedef struct PARSER_STRUCTURE {
    lexer_s* lexer;
    token_s* token;
} parser_s;

parser_s* parser_init(lexer_s* lexer);
// TODO: This is not going to be void
void parser_parse(parser_s* parser);

#endif
