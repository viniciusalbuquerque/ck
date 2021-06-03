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

void parser_next_token(parser_s* parser) {
    parser->token = lexer_next_token(parser->lexer);
}

void parser_skip_whitespace(parser_s* parser) {
    while(parser->token->type == TT_WS) {
        parser_next_token(parser);
    }
}

void parser_parse_expression(parser_s* parser) {
}

void parser_parse_statement(parser_s* parser) {
}

void parser_parse_function_args(parser_s* parser) {
    while (parser->token->type != TT_RP) {
        if (parser->token->type != TT_ID) {
            return;
        }

        parser_next_token(parser);
        parser_skip_whitespace(parser);
        if (parser->token->type != TT_COL) {
            printf("Error parsing function. Expected ':' and found %s\n", parser->token->value);
            exit(1);
        }
        parser_next_token(parser);
        parser_skip_whitespace(parser);
        if (parser->token->type != TT_TYPE) {
            printf("Error parsing function. Expected a type for your variable and found %s\n", parser->token->value);
            exit(1);
        }
        parser_next_token(parser);
        parser_skip_whitespace(parser);

        if (parser->token->type == TT_COMMA) {
            parser_next_token(parser);
        }
        parser_skip_whitespace(parser);
    }
}

void parser_parse_function(parser_s* parser) {
    if (parser->token->type != TT_FUN) {
        printf("Error parsing function. Expected 'fun' keyword. Found %s\n", parser->token->value);
        exit(1);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_ID) {
        printf("Expected a name for the function. Found %s\n", parser->token->value);
        exit(1);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_LP) {
        printf("Error parsing function. Expected '(' and found %s\n", parser->token->value);
        exit(1);
    }
    parser_next_token(parser);
    parser_parse_function_args(parser);

    if (parser->token->type != TT_RP) {
        printf("Error parsing function. Expected ')' and found %s\n", parser->token->value);
        exit(1);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_COL) {
        printf("Error parsing function. Expected ':' and found %s\n", parser->token->value);
        exit(1);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_TYPE) {
        printf("Error parsing function. Expected a return type and found %s\n", parser->token->value);
        exit(1);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_CURLY_LB) {
        printf("Error parsing function. Expected '{' and found %s\n", parser->token->value);
        exit(1);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    parser_parse_statement(parser);
    if (parser->token->type != TT_CURLY_RB) {
        printf("Error parsing function. Expected '}' and found %s\n", parser->token->value);
        exit(1);
    }
}

// TODO: It's not going to return void.
void parser_parse(parser_s* parser) {
    token_s* token = lexer_next_token(parser->lexer);
    parser->token = token;
    while (token->type != TT_EOF) {
        switch(token->type) {
            case TT_FUN:
                 parser_parse_function(parser);
                 break;
            default:
                exit(0);
        }
        printf("%s ", token_type_str(token->type));
        token = lexer_next_token(parser->lexer);
        if (token->type == TT_BAD_TOKEN) {
            exit(1);
        }
    }
    printf("%s\n", token_type_str(token->type));
}
