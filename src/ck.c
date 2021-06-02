#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/token.h"

void ck_compile(char* file_text) {
    int count = 0;
    lexer_s* lexer = lexer_init(file_text);
    token_s** tokens = lexer_tokenize(lexer);
    token_s* token = tokens[count];
    int type = token->type;
    while (type != TT_EOF) {
        printf("%s: %s\n", token_type_str(type), token->value);
        count++;
        token = tokens[count];
        type = token->type;
    }
    printf("%s\n", token_type_str(type));
}

char* ck_read_file(const char* filename) {
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Could not read the file: %s\n", filename);
        exit(1);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char* buffer = (char*) calloc(1, sizeof(char));
    buffer[0] = '\0';

    while((read = getline(&line, &len, fp)) != -1) {
        buffer = (char*) realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
        strcat(buffer, line);
    }
    fclose(fp);

    if (line)
        free(line);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("File not informed\n");
        return 1;
    }

    printf("argv[1]=%s\n", argv[1]);
    char* file_text = ck_read_file(argv[1]);
    ck_compile(file_text);
    return 0;
}
