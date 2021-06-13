#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/token.h"

char* single_chars = "{[()]};:,";

char* token_type_str_value(int type) {
    switch (type) {
        case TT_FUN:
            return "fun";
        case TT_RET:
            return "return";
        case TT_IF:
            return "if";
        case TT_FOR:
            return "for";
        case TT_WHILE:
            return "while";
        default:
            return "";
    }
}

int token_type_from_value(char curr) {
    switch (curr) {
        case '(': return TT_LP;
        case ')': return TT_RP;
        case '{': return TT_CURLY_LB;
        case '}': return TT_CURLY_RB;
        case '[': return TT_LB;
        case ']': return TT_RB;
        case ';': return TT_SEMI_COL;
        case ':': return TT_COL;
        case ',': return TT_COMMA;
        case '\'': return TT_SINGLE_QUOTE;
    }
    return TT_BAD_TOKEN;
}

int token_is_single_char(char curr) {
    for (int i = 0; i < strlen(single_chars); i++) {
        if (curr == single_chars[i]) return 1;
    }
    return 0;
}

char* token_type_str(int type) {
    switch (type) {
        case TT_FUN: return "TT_FUN";
        case TT_ID: return "TT_ID";
        case TT_LP: return "TT_LP";
        case TT_RP: return "TT_RP";
        case TT_CURLY_LB: return "TT_CURLY_LB";
        case TT_CURLY_RB: return "TT_CURLY_RB";
        case TT_COL: return "TT_COL";
        case TT_SEMI_COL: return "TT_SEMI_COL";
        case TT_WS: return "TT_WS";
        case TT_TYPE: return "TT_TYPE";
        case TT_NUMBER: return "TT_NUMBER";
        case TT_COMMA: return "TT_COMMA";
        case TT_RET: return "TT_RET";
        case TT_INCLUDE: return "TT_INCLUDE";
        case TT_EOF: return "TT_EOF";
        case TT_FOR: return "TT_FOR";
        case TT_WHILE: return "TT_WHILE";
        case TT_IF: return "TT_IF";
        case TT_REF: return "TT_REF";
        case TT_STAR: return "TT_STAR";
        case TT_SINGLE_QUOTE: return "TT_SINGLE_QUOTE";
        case TT_DOUBLE_QUOTE: return "TT_DOUBLE_QUOTE";
        case TT_CHAR: return "TT_CHAR";
        case TT_STRING: return "TT_STRING";
        case TT_BAD_TOKEN: 
        default: return "TT_BAD_TOKEN";
    }
}

int is_a_type(char* value) {
    return ((strcmp(value, "int") == 0) || (strcmp(value, "char") == 0));
}

int token_get_type_from_alphanum(char* value) {
    if (strcmp(value, token_type_str_value(TT_FUN)) == 0) {
        return TT_FUN;
    } else if(strcmp(value, token_type_str_value(TT_RET)) == 0) {
        return TT_RET;
    } else if(is_a_type(value)) {
        return TT_TYPE;
    } else if(strcmp(value, token_type_str_value(TT_IF)) == 0) {
        return TT_IF;
    }
    return TT_ID;
}

token_s* token_init(char* value, unsigned int pos, int type) {
    token_s* token = malloc(sizeof(token_s));
    token->value = value;
    token->pos = pos;
    token->type = type;
    printf("%s ", token_type_str(type));
    return token;
}

token_s* token_create_from_alphanum(char* value, unsigned int pos) {
    token_s* token = malloc(sizeof(token_s));
    token->pos = pos;
    token->value = value;
    token->type = token_get_type_from_alphanum(value);
    return token;
}
