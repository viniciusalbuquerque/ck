#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/token.h"

parser_s* parser_init(lexer_s* lexer) {
    parser_s* parser = malloc(sizeof(parser_s));
    parser->lexer = lexer;
    parser->token = 0; //NULL
    return parser;
}

// TODO: It's not going to be null
void parser_parse(parser_s* parser) {
    token_s* token = lexer_next_token(parser->lexer);
    while (token->type != TT_EOF) {
        printf("%s ", token_type_str(token->type));
        token = lexer_next_token(parser->lexer);
        if (token->type == TT_BAD_TOKEN) {
            exit(1);
        }
    }
    printf("%s\n", token_type_str(token->type));
}
