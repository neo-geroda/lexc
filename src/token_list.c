#include <stdlib.h>
#include <string.h>
#include "../include/token_list.h"
#include "../include/tokens.h"

static TokenNode *token_list_head = NULL;

static TokenNode *create_token_node(int key, const char *token) {
    TokenNode *node = (TokenNode *)malloc(sizeof(TokenNode));
    if (node) {
        node->key = key;
        node->token = token;
        node->next = NULL;
    }
    return node;
}

static void add_token(int key, const char *token) {
    TokenNode *new_node = create_token_node(key, token);
    if (!new_node) return;

    if (!token_list_head) {
        token_list_head = new_node;
    } else {
        TokenNode *current = token_list_head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void init_token_list(void) {
    if (token_list_head) return; 

    add_token(IDENT, "IDENT");
    add_token(ASSIGN_OP, "=");
    add_token(POS_OP, "+");
    add_token(NEG_OP, "-");
    add_token(LEFT_PAREN, "(");
    add_token(RIGHT_PAREN, ")");

    add_token(UNKNOWN_TOKEN, "UNKNOWN_TOKEN");

    add_token(ADDITION_OP, "+");
    add_token(SUBTRACT_OP, "-");
    add_token(MULTIPLY_OP, "*");
    add_token(DIVIDE_OP, "/");
    add_token(MODULO_OP, "%");
    add_token(EXPONENT_OP, "^");
    add_token(FLOORDIV_OP, "//");
    add_token(PRE_INCREMENT_OP, "++");
    add_token(POST_INCREMENT_OP, "++");
    add_token(PRE_DECREMENT_OP, "--");
    add_token(POST_DECREMENT_OP, "--");

    add_token(EQUALITY_OP, "==");
    add_token(INEQUALITY_OP, "!=");
    add_token(GREATER_THAN_OP, ">");
    add_token(LESS_THAN_OP, "<");
    add_token(GREATER_EQUAL_OP, ">=");
    add_token(LESS_EQUAL_OP, "<=");

    add_token(AND_OP, "and");
    add_token(OR_OP, "or");
    add_token(NOT_OP, "not");

    add_token(IF, "if");
    add_token(ELIF, "elif");
    add_token(ELSE, "else");
    add_token(REPEAT, "repeat");
    add_token(REPEATFOR, "repeatfor");
    add_token(STOP, "stop");
    add_token(CONTINUE, "continue");
    add_token(DISPLAY, "display");
    add_token(GET, "get");
    add_token(GIVE, "give");
    add_token(FUNCTION, "function");
    add_token(STATIC, "static");
    add_token(INCLUDE, "include");
    add_token(TRY, "try");
    add_token(BUNDLE, "bundle");
    add_token(VOID, "void");
    add_token(IN, "in");
    add_token(ONFAIL, "onfail");

    add_token(NUMBER, "number");
    add_token(DECIMAL, "decimal");
    add_token(SYMBOL, "symbol");
    add_token(BOOL, "bool");
    add_token(STRING, "string");
    add_token(LIST, "list");
    add_token(TEXT, "text");
    
    add_token(TRUE, "true");
    add_token(FALSE, "false");
    add_token(NOTHING, "nothing");
    add_token(NUM_LIT, "num_lit");
    add_token(DEC_LIT, "dec_lit");
    add_token(TEX_LIT, "tex_lit");
    add_token(SYM_LIT, "sym_lit");

    add_token(SEMICOLON, ";");
    add_token(COMMA, ",");
    add_token(LSQBRACKET, "[");
    add_token(RSQBRACKET, "]");
    add_token(LCBRACE, "{");
    add_token(RCBRACE, "}");
    add_token(DQUOTE, "\"");
    add_token(SQUOTE, "'");
    add_token(COLON, ":");

    add_token(NOISE_WORD, "NOISE_WORD");

    add_token(SINGLE_LINE_COMMENT, "SINGLE_LINE_COMMENT");
    add_token(MULTILINE_COMMENT, "MULTILINE_COMMENT");

    add_token(EOF_TOKEN, "EOF_TOKEN");
}

void cleanup_token_list(void) {
    TokenNode *current = token_list_head;
    while (current) {
        TokenNode *temp = current;
        current = current->next;
        free(temp);
    }
    token_list_head = NULL;
}

const char *token_name(int key) {
    TokenNode *current = token_list_head;
    while (current) {
        if (current->key == key) {
            return current->token;
        }
        current = current->next;
    }
    return "UNKNOWN";
}
