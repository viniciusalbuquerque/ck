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

void parser_parse_expression_number(parser_s* parser) {

}

void parser_parse_expression(parser_s* parser) {
    parser_skip_whitespace(parser);
    switch(parser->token->type) {
        case TT_ID:
        case TT_NUMBER:
            parser_next_token(parser);
            break;
        default:
            break;
    }
    parser_skip_whitespace(parser);
}

void print_and_exit(char* parsing_element, char* expected, char* found) {
    printf("Error parsing %s. Expected '%s' and found %s\n", parsing_element, expected, found);
    exit(1);
}

void parser_parse_conditional(parser_s* parser) {
    printf("parser_parse_conditional\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_IF) {
        print_and_exit("conditional", "if", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_LP) {
        print_and_exit("conditional", "(", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    parser_parse_expression(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_RP) {
        print_and_exit("conditional", ")", parser->token->value);
    }
    parser_next_token(parser);
    parser_parse_statement(parser);
}

void parser_parse_return(parser_s* parser) {
    printf("parser_parse_return\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_RET) {
        print_and_exit("return", "return", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    parser_parse_expression(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_SEMI_COL) {
        print_and_exit("return", ";", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
}

void parser_parser_variable(parser_s* parser) {
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_VAR) {
        print_and_exit("declaring variable", "var", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_ID) {
        print_and_exit("declaring variable", "a name for your variable", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);

    while (parser->token->type == TT_COMMA) {
        parser_next_token(parser);
        parser_skip_whitespace(parser);
        if (parser->token->type != TT_ID) {
            print_and_exit("declaring variable", "a name for your variable", parser->token->value);
        }
        parser_next_token(parser);
        parser_skip_whitespace(parser);
    }

    // For now it will be mandatory to declare type
    if (parser->token->type != TT_COL) {
        print_and_exit("declaring variable", ":", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_TYPE) {
        print_and_exit("declaring variable", "a type for your variable", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type == TT_ATTRIB) {
        parser_parse_expression(parser);
    }
    if (parser->token->type != TT_SEMI_COL) {
        print_and_exit("declaring variable", ";", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);

}

void parser_parse_compound(parser_s* parser) {
    printf("parser_parse_compound\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_CURLY_LB) {
        print_and_exit("compound", "{", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    parser_parse_statement(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_CURLY_RB) {
        print_and_exit("compound", "}", parser->token->value);
    }
}

void parser_parse_statement(parser_s* parser) {
    printf("parser_parse_statement\n");
    parser_skip_whitespace(parser);
    switch (parser->token->type) {
        case TT_CURLY_LB:
            parser_parse_compound(parser);
            break;
        case TT_IF:
            parser_parse_conditional(parser);
            break;
        case TT_RET:
            parser_parse_return(parser);
            break;
        case TT_VAR:
            parser_parser_variable(parser);
            break;
        default: // We need a NO-OP
            parser_parse_expression(parser);
            break;
    }
}

void parser_parse_function_args(parser_s* parser) {
    printf("parser_parse_function_args\n");
    while (parser->token->type != TT_RP) {
        if (parser->token->type != TT_ID) {
            return;
        }

        parser_next_token(parser);
        parser_skip_whitespace(parser);
        if (parser->token->type != TT_COL) {
            print_and_exit("function args", ":", parser->token->value);
        }
        parser_next_token(parser);
        parser_skip_whitespace(parser);
        if (parser->token->type != TT_TYPE) {
            print_and_exit("function args", "type for your variable", parser->token->value);
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
    printf("parser_parse_function\n");
    if (parser->token->type != TT_FUN) {
        print_and_exit("function", "fun", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_ID) {
        print_and_exit("function", "name for the function", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_LP) {
        print_and_exit("function", "(", parser->token->value);
    }

    parser_next_token(parser);
    parser_parse_function_args(parser);

    if (parser->token->type != TT_RP) {
        print_and_exit("function", ")", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_COL) {
        print_and_exit("function", ":", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_TYPE) {
        print_and_exit("function", "return type", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_CURLY_LB) {
        print_and_exit("function", "{", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    parser_parse_statement(parser);
    if (parser->token->type != TT_CURLY_RB) {
        print_and_exit("function", "}", parser->token->value);
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
