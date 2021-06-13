#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/parser.h"

parser_s* parser_init(lexer_s* lexer) {
    parser_s* parser = malloc(sizeof(parser_s));
    parser->lexer = lexer;
    parser->token = 0; //NULL
    return parser;
}

token_s* parser_get_current_token(parser_s* parser) {
    return parser->token = lexer_current_token(parser->lexer);
}

void parser_skip_whitespace(parser_s* parser) {
    while(parser->token->type == TT_WS) {
        //parser_next_token(parser);
    }
}

token_s* parser_eat_token(parser_s* parser, int token_type) {
    //parser_skip_whitespace(parser);
    if (parser->token->type != token_type) {
        printf("Parsing error. Did not expected: '%s'\n", parser->token->value);
        exit(1);
    }
    token_s* token = parser->token;
    token_s* token2 = parser_get_current_token(parser);
    printf("\neat: %s %s\n", token_type_str(token->type), token_type_str(token2->type));
    //parser_skip_whitespace(parser);
    return token;
}

ast_s* parser_parse_number(parser_s* parser) {
    printf("parser_parse_number\n");
    token_s* token_number = parser_eat_token(parser, TT_NUMBER);
    ast_s* ast_number = ast_init(AT_NUMBER);
    ast_number->ast_primitive = malloc(sizeof(ast_primitive));
    ast_number->ast_primitive->value = token_number->value;
    return ast_number;
}

ast_s* parser_parse_id(parser_s* parser) {
    printf("parser_parse_id\n");
    parser_eat_token(parser, TT_ID);

    ast_s* ast_id;
    if (parser->token->type == TT_ATTRIB) {
        ast_id = ast_init(AT_ASSIGNMENT);
        ast_add_child(ast_id, parser_parse_expression(parser));
    } else {
        ast_id = ast_init(AT_VARIABLE);
    }
    return ast_id;
}

ast_s* parser_parse_string(parser_s* parser) {
    printf("parser_parse_string\n");
    token_s* token_str = parser_eat_token(parser, TT_STRING);
    ast_s* ast_str = ast_init(AT_STRING);
    ast_str->ast_primitive = malloc(sizeof(ast_primitive));
    ast_str->ast_primitive->value = token_str->value;
    return ast_str;
}

ast_s* parser_parse_char(parser_s* parser) {
    printf("parser_parse_char\n");
    token_s* token_char = parser_eat_token(parser, TT_CHAR);
    ast_s* ast_char = ast_init(AT_CHAR);
    ast_char->ast_primitive = malloc(sizeof(ast_primitive));
    ast_char->ast_primitive->value = token_char->value;
    return ast_char;
}

ast_s* parser_parse_expression(parser_s* parser) {
    printf("parser_parse_expression\n");
    ast_s* child;
    switch(parser->token->type) {
        case TT_ID:
            child = parser_parse_id(parser);
            break;
        case TT_NUMBER:
            child = parser_parse_number(parser);
            break;
        case TT_CHAR:
            child = parser_parse_char(parser);
            break;
        case TT_STRING:
            child = parser_parse_string(parser);
            break;
        default:
            break;
    }
    ast_s* ast_exp = ast_init(AT_EXPRESSION);
    if (child) ast_add_child(ast_exp, child);
    return ast_exp;
}

ast_s* parser_parse_conditional(parser_s* parser) {
    printf("parser_parse_conditional\n");
    parser_eat_token(parser, TT_IF);

    ast_s* ast_conditional = ast_init(AT_CONDITIONAL);

    parser_eat_token(parser, TT_LP);
    ast_s* ast_exp = parser_parse_expression(parser);
    parser_eat_token(parser, TT_RP);
    ast_add_child(ast_conditional, ast_exp);
    ast_s* ast_stmt = parser_parse_statement(parser);
    ast_add_child(ast_conditional, ast_stmt);
    return ast_conditional;
}

ast_s* parser_parse_return(parser_s* parser) {
    printf("parser_parse_return\n");
    parser_eat_token(parser, TT_RET);
    ast_s* ast_ret = ast_init(AT_RETURN);
    ast_s* ast_exp = parser_parse_expression(parser);
    parser_eat_token(parser, TT_SEMI_COL);
    ast_add_child(ast_ret, ast_exp);
    return ast_ret;
}

ast_s* parser_parser_variable(parser_s* parser) {
    printf("parser_parser_variable");
    parser_eat_token(parser, TT_VAR);
    parser_eat_token(parser, TT_ID);

    while (parser->token->type == TT_COMMA) {
        parser_eat_token(parser, TT_COMMA);
        parser_eat_token(parser, TT_ID);
    }

    // For now it will be mandatory to declare type
    parser_eat_token(parser, TT_COL);
    parser_eat_token(parser, TT_TYPE);
    ast_s* ast_var_dec = ast_init(AT_VARIABLE_DEC);
    if (parser->token->type == TT_ATTRIB) {
        parser_eat_token(parser, TT_ATTRIB);
        ast_s* ast_exp = parser_parse_expression(parser);
        ast_add_child(ast_var_dec, ast_exp);
    }
    parser_eat_token(parser, TT_SEMI_COL);
    return ast_var_dec;
}

ast_s* parser_parse_compound(parser_s* parser) {
    printf("parser_parse_compound\n");
    parser_eat_token(parser, TT_CURLY_LB);
    ast_s* ast_compound = ast_init(AT_COMPOUND);
    ast_s* ast_stmt = parser_parse_statement(parser);
    parser_eat_token(parser, TT_CURLY_RB);
    ast_add_child(ast_compound, ast_stmt);
    return ast_compound;
}

ast_s* parser_parse_statement(parser_s* parser) {
    printf("parser_parse_statement\n");
    ast_s* ast_statement = ast_init(AT_STATEMENT);
    ast_s* ast_child = 0;
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

void parser_parse_function_args(parser_s* parser, ast_fun_def* ast_fun) {
    printf("parser_parse_function_args\n");
    parser_skip_whitespace(parser);
    while (parser->token->type != TT_RP) {
        token_s* token_id = parser_eat_token(parser, TT_ID);

        int args_size = ast_fun->args_size;
        ast_fun->args_size = args_size + 1;
        if (args_size == 0) {
            ast_fun->ast_args_list = malloc(sizeof(ast_fun_args*) * args_size);
        } else {
            ast_fun->ast_args_list = realloc(ast_fun->ast_args_list, sizeof(ast_fun_args*) * ast_fun->args_size);
        }

        ast_fun_args* ast_arg = malloc(sizeof(ast_fun_args));
        ast_arg->name = token_id->value;

        parser_eat_token(parser, TT_COL);
        token_s* token_arg_type = parser_eat_token(parser, TT_TYPE);
        ast_arg->type = token_arg_type->type; //TODO: get correct type.
        ast_fun->ast_args_list[ast_fun->args_size-1] = ast_arg;

        if (parser->token->type == TT_COMMA) {
            parser_eat_token(parser, TT_COMMA);
        }
    }
}

ast_s* parser_parse_function(parser_s* parser) {
    printf("parser_parse_function\n");
    parser_eat_token(parser, TT_FUN);
    ast_s* ast_fun = ast_init(AT_FUNCTION_DEFINITION);
    ast_fun->ast_fun_def = malloc(sizeof(ast_fun_def));
    token_s* token_fun_name = parser_eat_token(parser, TT_ID);
    ast_fun->ast_fun_def->name = token_fun_name->value;
    parser_eat_token(parser, TT_LP);
    parser_parse_function_args(parser, ast_fun->ast_fun_def);
    parser_eat_token(parser, TT_RP);

    if (parser->token->type == TT_COL) {
        parser_eat_token(parser, TT_COL);
        token_s* token_ret_type = parser_eat_token(parser, TT_TYPE);
        ast_fun->ast_fun_def->return_type = token_ret_type->type; //TODO: get the return type. Maybe we don't need the token kind TT_TYPE?
    }

    if (parser->token->type != TT_CURLY_LB) {
        parser_eat_token(parser, TT_CURLY_LB);
        // This is supposed to throw the error
    }
    ast_s* ast_statement = parser_parse_statement(parser);
    ast_add_child(ast_fun, ast_statement);
    printf("\n%c %d\n", parser->token->value[0], parser->token->value[0]);
    return ast_fun;
}

ast_s* parser_parse(parser_s* parser) {
    ast_s* ast = ast_init(AT_PROGRAM);
    token_s* token = 0;
    while ((token = parser_get_current_token(parser))->type != TT_EOF) {
        ast_s* child = 0;
        switch(token->type) {
            case TT_FUN:
                child = parser_parse_function(parser);
                break;
            default:
                break;
        }
        lexer_next(parser->lexer);
        if (child) ast_add_child(ast, child);
    }
    printf("parser_parse done\n");
    return ast;
}
