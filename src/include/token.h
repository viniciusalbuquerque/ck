#ifndef CK_TOKEN_H
#define CK_TOKEN_H
typedef struct TOKEN_STRUCT {
    unsigned int pos;
    char* value;
    enum {
        TT_ID,
        TT_INCLUDE,
        TT_FUN,
        TT_VAL,
        TT_VAR,
        TT_NUMBER,
        TT_COL,
        TT_SEMI_COL,
        TT_EQ,
        TT_CURLY_LB,
        TT_CURLY_RB,
        TT_LB,
        TT_RB,
        TT_LP,
        TT_RP,
        TT_TYPE,
        TT_RET,
        TT_WS,
        TT_BAD_TOKEN,
        TT_EOF,
    } type;
} token_s;

token_s* token_init(char* value, unsigned int pos, int type);
token_s* token_create_from_alphanum(char* value, unsigned int pos);
char* token_type_str(int type);
int token_type_from_value(char curr);
int token_is_single_char(char curr);
#endif
