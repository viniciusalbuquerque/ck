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
    if (position + 1 >= text_size) {
        printf("End of text\n");
        return;
    }

    lexer->pos = position + 1;
    lexer->current = text[lexer->pos];
}

int is_whitespace(char c) {
    return c == '\t' || c == ' ' || c == '\n';
}

token_s** lexer_tokenize(lexer_s* lexer) {
    token_s** tokens = malloc(sizeof(token_s*));
    size_t tokens_size = 0;
    while(lexer->current != '\0') {
        char c = lexer->current;
        token_s* token;
        if (c == '(') {
            char* value = malloc(sizeof(char));
            *value = c;
            token = token_init(value, lexer->pos, TT_LB);
        } else if (c == ')') {
            char* value = malloc(sizeof(char));
            *value = c;
            token = token_init(value, lexer->pos, TT_RB);
        } else if (isalnum(lexer->current)) {
            size_t start = lexer->pos;
            while(isalnum(lexer->current)) {
                lexer_next(lexer);
            }
            size_t end = lexer->pos;
            char* value = malloc(sizeof(char) * (end - start + 1));
            strncpy(value, lexer->text + start, end - start);
            token = token_create_from_alphanum(value, start);
        } else if (is_whitespace(lexer->current)) {
            size_t start = lexer->pos;
            while(is_whitespace(lexer->current)) {
                lexer_next(lexer);
            }
            size_t end = lexer->pos;
            char* value = malloc(sizeof(char) * (end - start + 1));
            strncpy(value, lexer->text + start, end - start);
            token = token_init(value, start, TT_WS);
        } else {
            char* value = malloc(sizeof(char));
            *value = c;
            token = token_init(value, lexer->pos, TT_BAD_TOKEN);
        }
        tokens_size += 1;
        tokens = realloc(tokens, tokens_size);
        tokens[tokens_size-1] = token;
    }
    return tokens;
}

