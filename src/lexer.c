#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lexer.h"
#include "include/token.h"

lexer_s* lexer_init(char* text) {
    lexer_s* lexer = malloc(sizeof(lexer_s));
    lexer->text = text;
    lexer->text_size = strlen(text);
    lexer->pos = 0;
    lexer->current = lexer->text[lexer->pos];
    return lexer;
}

void lexer_next(lexer_s* lexer) {
    unsigned int position = lexer->pos;
    char* text = lexer->text;
    size_t text_size = lexer->text_size;
    if (position + 1 > text_size) {
        printf("End of text\n");
        return;
    }

    lexer->pos = position + 1;
    lexer->current = text[lexer->pos];
}

void lexer_previous(lexer_s* lexer) {
    unsigned int position = lexer->pos;
    char* text = lexer->text;
    if (position - 1 < 0) {
        printf("Can't go back under zero\n");
        return;
    }

    lexer->pos = position - 1;
    lexer->current = text[lexer->pos];
}

int is_whitespace(char c) {
    return isspace(c) || c == '\t' || c == 10 || c == 13;
}

char* substr(char* text, unsigned int start, unsigned int end) {
    size_t value_length = end - start + 1;
    char* value = malloc(sizeof(char) * value_length + 1);
    memset(value, '\0', value_length + 1);
    value = strncpy(value, text + start, value_length);
    return value;
}

token_s* lexer_parse_number_token(lexer_s* lexer) {
    int start = lexer->pos;
    while(isdigit(lexer->current)) {
        lexer_next(lexer);
    }
    int end = lexer->pos - 1;

    char* number = substr(lexer->text, start, end);
    return token_init(number, start, TT_NUMBER);
}

token_s* lexer_parse_alpha(lexer_s* lexer) {
    int start = lexer->pos;
    while(isalnum(lexer->current)) {
        lexer_next(lexer);
    }
    int end = lexer->pos - 1;
    char* value = substr(lexer->text, start, end);
    return token_init(value, start, token_get_type_from_alphanum(value));
}

token_s* lexer_parse_single_char(lexer_s* lexer) {
    char* value = malloc(sizeof(char) + 1);
    memset(value, '\0', 2);
    *value = lexer->current;
    return token_init(value, lexer->pos, token_type_from_value(lexer->current));
}

token_s* lexer_parse_whitespace(lexer_s* lexer) {
    int start = lexer->pos;
    while(is_whitespace(lexer->current)) {
        lexer_next(lexer);
    }
    int end = lexer->pos - 1;
    char* ws = substr(lexer->text, start, end);
    return token_init(ws, lexer->pos, TT_WS);
}

token_s** lexer_tokenize(lexer_s* lexer) {
    token_s** tokens = malloc(sizeof(token_s*));
    size_t tokens_size = 0;
    while(lexer->current != '\0') {
        char c = lexer->current;
        token_s* token;
        if (token_is_single_char(c)) {
            token = lexer_parse_single_char(lexer);
            lexer_next(lexer);
        } else if (isdigit(lexer->current)) {
            token = lexer_parse_number_token(lexer);
        } else if (isalpha(lexer->current)) {
            token = lexer_parse_alpha(lexer);
        } else if (is_whitespace(lexer->current)) {
            token = lexer_parse_whitespace(lexer);
        } else {
            char* value = malloc(sizeof(char));
            *value = c;
            token = token_init(value, lexer->pos, TT_BAD_TOKEN);
        }
        tokens_size += 1;
        tokens = realloc(tokens, tokens_size * sizeof(token_s*));
        tokens[tokens_size-1] = token;
    }
    tokens_size += 1;
    tokens = realloc(tokens, tokens_size * sizeof(token_s*));
    char* value = malloc(sizeof(char));
    *value = '\0';
    tokens[tokens_size-1] = token_init(value, lexer->pos, TT_EOF);
    return tokens;
}

