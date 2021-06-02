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

token_s** lexer_tokenize(lexer_s* lexer) {
    token_s** tokens = malloc(sizeof(token_s*));
    size_t tokens_size = 0;
    while(lexer->current != '\0') {
        char c = lexer->current;
        token_s* token;
        if (token_is_single_char(c)) {
            char* value = malloc(sizeof(char) + 1);
            memset(value, '\0', 2);
            *value = c;
            token = token_init(value, lexer->pos, token_type_from_value(c));
        } else if (isalnum(lexer->current)) {
            int isalpha = isalpha(lexer->current);
            size_t start = lexer->pos;
            if (isalpha) {
                while(isalnum(lexer->current)) {
                    lexer_next(lexer);
                }
            } else {
                while(isdigit(lexer->current)) {
                    lexer_next(lexer);
                }
            }
            lexer_previous(lexer);
            size_t end = lexer->pos;
            if (start > 0) start--;
            size_t size = end - start + 1;
            char* value = malloc(sizeof(char) * (size));
            memset(value, '\0', strlen(value));
            strncpy(value, lexer->text + start, size);
            if (isalpha) {
                token = token_create_from_alphanum(value, start);
            } else {
                token = token_init(value, start, TT_NUMBER);
            }
        } else if (is_whitespace(lexer->current)) {
            size_t start = lexer->pos;
            while(is_whitespace(lexer->current)) {
                lexer_next(lexer);
            }
            lexer_previous(lexer);
            size_t end = lexer->pos;
            size_t size = end - start + 1;
            char* value = malloc(sizeof(char) * (size));
            memset(value, '\0', strlen(value));
            strncpy(value, lexer->text + start, size);
            token = token_init(value, start, TT_WS);
        } else {
            char* value = malloc(sizeof(char));
            *value = c;
            token = token_init(value, lexer->pos, TT_BAD_TOKEN);
        }
        tokens_size += 1;
        tokens = realloc(tokens, tokens_size * sizeof(token_s*));
        tokens[tokens_size-1] = token;
        lexer_next(lexer);
    }
    tokens_size += 1;
    tokens = realloc(tokens, tokens_size * sizeof(token_s*));
    char* value = malloc(sizeof(char));
    *value = '\0';
    tokens[tokens_size-1] = token_init(value, lexer->pos, TT_EOF);
    return tokens;
}

