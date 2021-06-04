#ifndef CK_PARSER_H
#define CK_PARSER_H
#include "ast.h"
#include "lexer.h"
#include "token.h"

typedef struct PARSER_STRUCTURE {
    lexer_s* lexer;
    token_s* token;
} parser_s;

parser_s* parser_init(lexer_s* lexer);
ast_s* parser_parse(parser_s* parser);
ast_s* parser_parse_expression(parser_s* parser);
token_s* parser_next_token(parser_s* parser);
ast_s* parser_parse_statement(parser_s* parser);
#endif
