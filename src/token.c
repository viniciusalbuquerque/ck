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
        case TT_BAD_TOKEN: 
        default: return "TT_BAD_TOKEN";
    }
}

int is_a_type(char* value) {
    return ((strcmp(value, "int") == 0) || (strcmp(value, "char") == 0));
}

int token_get_type_from_alphanum(char* value) {
    if (strncmp(value, token_type_str_value(TT_FUN), strlen(value)) == 0) {
        return TT_FUN;
    } else if(strcmp(value, token_type_str_value(TT_RET)) == 0) {
        return TT_RET;
    } else if(is_a_type(value)) {
        return TT_TYPE;
    }
    return TT_ID;
}

token_s* token_init(char* value, unsigned int pos, int type) {
    token_s* token = malloc(sizeof(token_s));
    token->value = value;
    token->pos = pos;
    token->type = type;
    return token;
}

token_s* token_create_from_alphanum(char* value, unsigned int pos) {
    token_s* token = malloc(sizeof(token_s));
    token->pos = pos;
    token->value = value;
    token->type = token_get_type_from_alphanum(value);
    return token;
}
