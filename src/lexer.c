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

void lexer_skip_whitespace(lexer_s* lexer) {
    token_s* token = lexer_next_token(lexer);
    while (token->type == TT_WS) {
        token = lexer_next_token(lexer);
    }
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
    lexer_next(lexer);
    return token_init(value, lexer->pos, token_type_from_value(value[0]));
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

token_s* lexer_parse_char(lexer_s* lexer) {

    if (lexer->current != '\'') {
        printf("Error tokeninzing char. Expected '\'' and found %c\n", lexer->current);
        exit(1);
    }
    int start = lexer->pos;
    lexer_next(lexer);
    char c = lexer->current;
    lexer_next(lexer);

    if (lexer->current != '\'') {
        printf("Error tokeninzing char. Expected '\'' and found %c\n", lexer->current);
        exit(1);
    }

    char* value = malloc(sizeof(char) + 1);
    memset(value, '\0', 2);
    *value = c;
    lexer_next(lexer);
    return token_init(value, start, TT_CHAR);
}

token_s* lexer_parse_string(lexer_s* lexer) {
    if (lexer->current != '\"') {
        printf("Error tokeninzing string. Expected '\"' and found %c\n", lexer->current);
        exit(1);
    }
    int start = lexer->pos;
    lexer_next(lexer);
    while((lexer->current) != '\"') {
        lexer_next(lexer);
    }
    int end = lexer->pos - 1;
    char* str = substr(lexer->text, start + 1, end);
    lexer_next(lexer);
    return token_init(str, lexer->pos, TT_STRING);
}

token_s* lexer_next_token(lexer_s* lexer) {
    if (lexer->current == '\0') {
        char* value = malloc(sizeof(char));
        *value = '\0';
        return token_init(value, lexer->pos, TT_EOF);
    }

    char c = lexer->current;
    if (token_is_single_char(c)) {
        return lexer_parse_single_char(lexer);
    } else if (c == '\'') {
        return lexer_parse_char(lexer);
    } else if (c == '\"') {
        return lexer_parse_string(lexer);
    } else if (isdigit(lexer->current)) {
        return lexer_parse_number_token(lexer);
    } else if (isalpha(lexer->current)) {
        return lexer_parse_alpha(lexer);
    } else if (is_whitespace(lexer->current)) {
        return lexer_parse_whitespace(lexer);
    } 
    printf("Was not expecting %c at position %d.\n", c, lexer->pos);
    exit(1);
    /*char* value = malloc(sizeof(char));
    *value = c;
    return token_init(value, lexer->pos, TT_BAD_TOKEN);*/
}

