#include "list.h"
#include <stdlib.h>

typedef struct RETURN {
    struct AST_EXPRESSION* ast_expression;
} Return;

typedef struct AST_COMPOUND {
    struct AST_STATEMENT* ast_statement;
} Compound;

typedef struct AST_EXPRESSION {
    struct AST_EXPRESSION* ast_expression;
    struct AST_PRIMITIVE* ast_primitive;
    struct AST_VARIABLE* ast_variable;
} Expression;

typedef struct AST_STATEMENT {
    struct AST_EXPRESSION* ast_expression;
    struct AST_CONDITIONAL* ast_conditional;
    struct AST_COMPOUND* ast_compound;
    struct AST_VARIABLE* ast_variable;
    struct AST_RETURN* ast_return;
} Statement;

typedef struct AST_CONDITIONAL {
    struct AST_EXPRESSION* ast_expression;
    struct AST_STATEMENT* ast_statement;
} Conditional;

typedef struct AST_FUN_DEF_STRUCT {
    char* name;
    list* args_list;
    size_t args_size;
    int return_type;
    struct AST_STATEMENT* ast_stmt;
} Fun_definition;

typedef struct AST_FUN_ARG {
    char* name;
    char* value;
    int type;
} Fun_arg;

typedef struct AST_VARIABLE {
    char* name;
    int type;
} Variable;

typedef struct AST_PRIMITIVE {
    char* value;
    int type;
} Primitive;

typedef struct AST_ASSIGNMENT {
    char* name;
    Expression* expression;
} Assignment;

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
    list* children;
    size_t children_size;

    Fun_definition* fun_def;
    Variable* variable;
    Primitive* primitive;
    Assignment* assignment;
    Statement* statement;
    Expression* expression;
    Conditional* conditional;
    Return* return_exp;
    Compound* compound;

} ast_s;

ast_s* ast_init(int type);
void ast_add_child(ast_s* ast, ast_s* child);
void ast_print(ast_s* ast, int level);


