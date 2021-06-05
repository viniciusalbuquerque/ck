#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"

parser_s* parser_init(lexer_s* lexer) {
    parser_s* parser = malloc(sizeof(parser_s));
    parser->lexer = lexer;
    parser->token = 0; //NULL
    return parser;
}

token_s* parser_next_token(parser_s* parser) {
    return parser->token = lexer_next_token(parser->lexer);
}

void parser_skip_whitespace(parser_s* parser) {
    while(parser->token->type == TT_WS) {
        parser_next_token(parser);
    }
}

ast_s* parser_parse_expression(parser_s* parser) {
    printf("parser_parse_expression\n");
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
    return ast_init(AT_EXPRESSION);
}

void print_and_exit(char* parsing_element, char* expected, char* found) {
    printf("Error parsing %s. Expected '%s' and found %s\n", parsing_element, expected, found);
    exit(1);
}

ast_s* parser_parse_conditional(parser_s* parser) {
    printf("parser_parse_conditional\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_IF) {
        print_and_exit("conditional", "if", parser->token->value);
    }

    ast_s* ast_conditional = ast_init(AT_CONDITIONAL);

    parser_next_token(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_LP) {
        print_and_exit("conditional", "(", parser->token->value);
    }

    parser_next_token(parser);
    parser_skip_whitespace(parser);
    ast_s* ast_exp = parser_parse_expression(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_RP) {
        print_and_exit("conditional", ")", parser->token->value);
    }

    ast_add_child(ast_conditional, ast_exp);
    parser_next_token(parser);
    ast_s* ast_stmt = parser_parse_statement(parser);
    ast_add_child(ast_conditional, ast_stmt);
    return ast_conditional;
}

ast_s* parser_parse_return(parser_s* parser) {
    printf("parser_parse_return\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_RET) {
        print_and_exit("return", "return", parser->token->value);
    }
    ast_s* ast_ret = ast_init(AT_RETURN);
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    ast_s* ast_exp = parser_parse_expression(parser);
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_SEMI_COL) {
        print_and_exit("return", ";", parser->token->value);
    }
    ast_add_child(ast_ret, ast_exp);
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    return ast_ret;
}

ast_s* parser_parser_variable(parser_s* parser) {
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_VAR) {
        print_and_exit("declaring variable", "var", parser->token->value);
    }

    ast_s* ast_var_dec = ast_init(AT_VARIABLE);

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
        ast_s* ast_exp = parser_parse_expression(parser);
        ast_add_child(ast_var_dec, ast_exp);
    }
    if (parser->token->type != TT_SEMI_COL) {
        print_and_exit("declaring variable", ";", parser->token->value);
    }
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    return ast_var_dec;
}

ast_s* parser_parse_compound(parser_s* parser) {
    printf("parser_parse_compound\n");
    parser_skip_whitespace(parser);
    if (parser->token->type != TT_CURLY_LB) {
        print_and_exit("compound", "{", parser->token->value);
    }

    ast_s* ast_compound = ast_init(AT_COMPOUND);
    parser_next_token(parser);
    parser_skip_whitespace(parser);
    ast_s* ast_stmt = parser_parse_statement(parser);
    parser_skip_whitespace(parser);

    if (parser->token->type != TT_CURLY_RB) {
        print_and_exit("compound", "}", parser->token->value);
    }

    ast_add_child(ast_compound, ast_stmt);
    return ast_compound;
}

ast_s* parser_parse_statement(parser_s* parser) {
    printf("parser_parse_statement\n");
    ast_s* ast_statement = ast_init(AT_STATEMENT);
    ast_s* ast_child = 0;
    parser_skip_whitespace(parser);
    switch (parser->token->type) {
        case TT_CURLY_LB:
            ast_child = parser_parse_compound(parser);
            break;
        case TT_IF:
            ast_child = parser_parse_conditional(parser);
            break;
        case TT_RET:
            ast_child = parser_parse_return(parser);
            break;
        case TT_VAR:
            ast_child = parser_parser_variable(parser);
            break;
        default: // We need a NO-OP
            ast_child = parser_parse_expression(parser);
            break;
    }
    ast_add_child(ast_statement, ast_child);
    return ast_statement;
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

ast_s* parser_parse_function(parser_s* parser) {
    printf("parser_parse_function\n");
    if (parser->token->type != TT_FUN) {
        print_and_exit("function", "fun", parser->token->value);
    }
    ast_s* ast_fun = ast_init(AT_FUNCTION_DEFINITION);
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
    ast_s* ast_statement = parser_parse_statement(parser);
    ast_add_child(ast_fun, ast_statement);
    if (parser->token->type != TT_CURLY_RB) {
        print_and_exit("function", "}", parser->token->value);
    }
    return ast_fun;
}

ast_s* parser_parse(parser_s* parser) {
    ast_s* ast = ast_init(AT_PROGRAM);
    token_s* token = 0;
    while ((token = parser_next_token(parser))->type != TT_EOF) {
        ast_s* child = 0;
        switch(token->type) {
            case TT_FUN:
                child = parser_parse_function(parser);
                break;
            default:
                parser_skip_whitespace(parser);
                break;
        }
        token = lexer_next_token(parser->lexer);

        if (token->type == TT_BAD_TOKEN) {
            printf("\nBAD_TOKEN. Exiting\n");
            exit(1);
        }
        if (child) ast_add_child(ast, child);
    }
    printf("parser_parse done\n");
    return ast;
}
