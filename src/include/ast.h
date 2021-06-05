#include <stdlib.h>
typedef struct AST_STRUCT {
    enum {
        AT_PROGRAM,
        AT_FUNCTION_DEFINITION,
        AT_COMPOUND,
        AT_STATEMENT,
        AT_EXPRESSION,
        AT_VARIABLE,
        AT_RETURN,
        AT_CONDITIONAL,
    } type;
    struct AST_STRUCT** children;
    size_t children_size;
} ast_s;

ast_s* ast_init(int type);
void ast_add_child(ast_s* ast, ast_s* child);
void ast_print(ast_s* ast, int level);
