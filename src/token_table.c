/* token_table.c
 * Defines the token lookup table and provides token_name()
 */
#include <string.h>
#include "../include/tokens.h"

/* Table of token keys and their canonical names */
const TokenEntry token_table[] = {
    {IDENT, "IDENT"},
    {ASSIGN_OP, "ASSIGN_OP"},
    {POS_OP, "POS_OP"},
    {NEG_OP, "NEG_OP"},
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},

    {UNKNOWN_TOKEN, "UNKNOWN_TOKEN"},

    {ADDITION_OP, "ADDITION_OP"},
    {SUBTRACT_OP, "SUBTRACT_OP"},
    {MULTIPLY_OP, "MULTIPLY_OP"},
    {DIVIDE_OP, "DIVIDE_OP"},
    {MODULO_OP, "MODULO_OP"},
    {EXPONENT_OP, "EXPONENT_OP"},
    {FLOORDIV_OP, "FLOORDIV_OP"},
    {PRE_INCREMENT_OP, "PRE_INCREMENT_OP"},
    {POST_INCREMENT_OP, "POST_INCREMENT_OP"},
    {PRE_DECREMENT_OP, "PRE_DECREMENT_OP"},
    {POST_DECREMENT_OP, "POST_DECREMENT_OP"},

    {EQUALITY_OP, "EQUALITY_OP"},
    {INEQUALITY_OP, "INEQUALITY_OP"},
    {GREATER_THAN_OP, "GREATER_THAN_OP"},
    {LESS_THAN_OP, "LESS_THAN_OP"},
    {GREATER_EQUAL_OP, "GREATER_EQUAL_OP"},
    {LESS_EQUAL_OP, "LESS_EQUAL_OP"},

    {AND_OP, "AND_OP"},
    {OR_OP, "OR_OP"},
    {NOT_OP, "NOT_OP"},

    {IF, "IF"},
    {ELIF, "ELIF"},
    {ELSE, "ELSE"},
    {REPEAT, "REPEAT"},
    {REPEATFOR, "REPEATFOR"},
    {STOP, "STOP"},
    {CONTINUE, "CONTINUE"},
    {DISPLAY, "DISPLAY"},
    {GET, "GET"},
    {GIVE, "GIVE"},
    {FUNCTION, "FUNCTION"},
    {STATIC, "STATIC"},
    {INCLUDE, "INCLUDE"},
    {TRY, "TRY"},
    {BUNDLE, "BUNDLE"},
    {VOID, "VOID"},
    {IN, "IN"},
    {ONFAIL, "ONFAIL"},

    {NUMBER, "NUMBER"},
    {DECIMAL, "DECIMAL"},
    {SYMBOL, "SYMBOL"},
    {BOOL, "BOOL"},
    {STRING, "STRING"},
    {LIST, "LIST"},
    {TEXT, "TEXT"},

    {TRUE, "TRUE"},
    {FALSE, "FALSE"},
    {NOTHING, "NOTHING"},
    {NUM_LIT, "NUM_LIT"},
    {DEC_LIT, "DEC_LIT"},
    {TEX_LIT, "TEX_LIT"},
    {SYM_LIT, "SYM_LIT"},

    {SEMICOLON, "SEMICOLON"},
    {COMMA, "COMMA"},
    {LSQBRACKET, "LSQBRACKET"},
    {RSQBRACKET, "RSQBRACKET"},
    {LCBRACE, "LCBRACE"},
    {RCBRACE, "RCBRACE"},
    {DQUOTE, "DQUOTE"},
    {SQUOTE, "SQUOTE"},
    {COLON, "COLON"},

    {NOISE_WORD, "NOISE_WORD"},

    {SINGLE_LINE_COMMENT, "SINGLE_LINE_COMMENT"},
    {MULTILINE_COMMENT, "MULTILINE_COMMENT"},

    {EOF_TOKEN, "EOF_TOKEN"}
};

const int token_table_size = sizeof(token_table) / sizeof(token_table[0]);

const char *token(int key) {
    for (int i = 0; i < token_table_size; ++i) {
        if (token_table[i].key == key) return token_table[i].token;
    }
    return "UNKNOWN";
}
