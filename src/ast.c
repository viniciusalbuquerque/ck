#include <stdlib.h>
#include <stdio.h>
#include "include/ast.h"

ast_s* ast_init(int type) {
    ast_s* ast = malloc(sizeof(ast_s));
    ast->type = type;
    ast->children = 0;
    ast->children_size = 0;
    return ast;
}

char* ast_type_to_str(ast_s* ast) {
    switch (ast->type) {
        case AT_PROGRAM: return "AT_PROGRAM";
        case AT_FUNCTION_DEFINITION: return "AT_FUNCTION_DEFINITION";
        case AT_COMPOUND: return "AT_COMPOUND";
        case AT_STATEMENT: return "AT_STATEMENT";
        case AT_EXPRESSION: return "AT_EXPRESSION";
        case AT_VARIABLE: return "AT_VARIABLE";
        case AT_RETURN: return "AT_RETURN";
        case AT_CONDITIONAL: return "AT_CONDITIONAL";
    }
    return "AT_NOT_FOUND";
}

void ast_add_child(ast_s* ast, ast_s* child) {
    if (!child) return;
    if (ast->type == AT_PROGRAM) {
        printf("add to %s a %s\n", ast_type_to_str(ast), ast_type_to_str(child));
    }
    ast->children_size += 1;
    if (!ast->children) {
        ast->children = malloc(sizeof(ast_s*) * ast->children_size);
    } else {
        ast->children = realloc(ast->children, sizeof(ast_s*) * ast->children_size);
    }
    ast->children[ast->children_size - 1] = child;
}

void ast_print(ast_s* ast, int level) {
    if (!ast) return;
    if (level != 0) {
        printf("|");
        for (int i = 0; i < level; i++) printf("_");
    }
    printf("%s: %d\n", ast_type_to_str(ast), (int)ast->children_size);
    if (!ast->children) return;
    for (int i = 0; i < ast->children_size; i++) {
        ast_print(ast->children[i], level + 1);
    }
}
