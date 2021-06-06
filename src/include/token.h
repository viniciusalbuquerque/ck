#ifndef CK_TOKEN_H
#define CK_TOKEN_H
typedef struct TOKEN_STRUCT {
    unsigned int pos;
    char* value;
    enum {
        TT_ID, // a string
        TT_INCLUDE, // #include
        TT_FUN, // fun
        TT_REF, // &
        TT_STAR, // *
        TT_VAL, // val
        TT_VAR, // var
        TT_FOR, // for
        TT_WHILE, // while
        TT_IF, // if
        TT_ATTRIB, // =
        TT_NUMBER, // a number
        TT_COMMA, // ,
        TT_SINGLE_QUOTE, // "
        TT_COL, // :
        TT_SEMI_COL, // ;
        TT_EQ, // ==
        TT_CURLY_LB, // {
        TT_CURLY_RB, // }
        TT_LB, // [
        TT_RB, // ]
        TT_LP, // (
        TT_RP, // )
        TT_TYPE, // type
        TT_RET, // return
        TT_WS, // whitespace
        TT_BAD_TOKEN, //a bad todken (don't know if I'm going to use it
        TT_EOF, // eof
    } type;
} token_s;

token_s* token_init(char* value, unsigned int pos, int type);
token_s* token_create_from_alphanum(char* value, unsigned int pos);
char* token_type_str(int type);
int token_type_from_value(char curr);
int token_is_single_char(char curr);
int token_get_type_from_alphanum(char* alphanum);
#endif
