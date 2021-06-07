#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/token.h"
#include "include/parser.h"

void ck_compile(char* file_text) {
    lexer_s* lexer = lexer_init(file_text);
    parser_s* parser = parser_init(lexer);
    ast_s* ast = parser_parse(parser);
    ast_print(ast, 0);
}

char* ck_read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not read the file: %s\n", filename);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*) malloc(sizeof(char) * (file_size));
    if (!buffer) {
        fclose(file);
        printf("Could not allocate buffer for file\n");
        exit(1);
    }

    fread(buffer, 1, file_size, file);
    fclose(file);
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
    free(file_text);
    return 0;
}
