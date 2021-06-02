#include <stdlib.h>
#include <string.h>
#include "include/token.h"

char* token_type_to_str_value(int type) {
    switch (type) {
        case TT_FUN:
            return "fun";
        default:
            return "";
    }
}

int token_get_type_from_alphanum(char* value) {
    if (strncmp(value, token_type_to_str_value(TT_FUN), strlen(value)) == 0) {
        return TT_FUN;
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
