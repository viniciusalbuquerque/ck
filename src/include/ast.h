#include <stdlib.h>

typedef struct AST_STRUCT {
    enum {
        AT_PROGRAM,
        AT_FUNCTION_DEFINITION,
        AT_COMPOUND,
        AT_STATEMENT,
        AT_EXPRESSION,
        AT_VARIABLE,
        AT_VARIABLE_DEC,
        AT_ASSIGNMENT,
        AT_NUMBER,
        AT_CHAR,
        AT_STRING,
        AT_RETURN,
        AT_CONDITIONAL,
        AT_ARGS,
    } type;
    struct AST_STRUCT** children;
    size_t children_size;

    struct AST_FUN_DEF_STRUCT* ast_fun_def;
    struct AST_VARIABLE* ast_variable;
    struct AST_PRIMITIVE* ast_primitive;
    struct AST_ASSIGNMENT* ast_assignment;
    struct AST_STATEMENT* ast_stmt;
    struct AST_EXPRESSION* ast_expression;
    struct AST_CONDITIONAL* ast_conditional;
    struct AST_RETURN* ast_return;
    struct AST_COMPOUND* ast_compound;

} ast_s;

typedef struct AST_RETURN {
    struct AST_EXPRESSION* ast_expression;
} ast_return;

typedef struct AST_COMPOUND {
    struct AST_STATEMENT* ast_statement;
} ast_compound;

typedef struct AST_EXPRESSION {
    struct AST_EXPRESSION* ast_expression;
    struct AST_PRIMITIVE* ast_primitive;
    struct AST_VARIABLE* ast_variable;
} ast_expression;

typedef struct AST_STATEMENT {
    struct AST_EXPRESSION* ast_expression;
    struct AST_CONDITIONAL* ast_conditional;
    struct AST_COMPOUND* ast_compound;
    struct AST_VARIABLE* ast_variable;
    struct AST_RETURN* ast_return;
} ast_statement;

typedef struct AST_CONDITIONAL {
    struct AST_EXPRESSION* ast_expression;
    struct AST_STATEMENT* ast_statement;
} ast_conditional;

typedef struct AST_FUN_DEF_STRUCT {
    char* name;
    struct AST_FUN_ARGS** ast_args_list;
    size_t args_size;
    int return_type;
    struct AST_STATEMENT* ast_stmt;
} ast_fun_def;

typedef struct AST_FUN_ARGS {
    char* name;
    char* value;
    int type;
} ast_fun_args;

typedef struct AST_VARIABLE {
    char* name;
    int type;
} ast_variable;

typedef struct AST_PRIMITIVE {
    char* value;
    int type;
} ast_primitive;

typedef struct AST_ASSIGNMENT {
    char* name;
    char* value;
    int type;
} ast_assignment;

ast_s* ast_init(int type);
void ast_add_child(ast_s* ast, ast_s* child);
void ast_print(ast_s* ast, int level);
