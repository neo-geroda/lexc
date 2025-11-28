#include <string.h>
#include "../include/token_stream.h"
#include "../include/lexer.h"
#include "../include/token_list.h"

static Token peeked_token;
static int has_peeked = 0;

void init_lexer_stream(FILE *in) {
    init_token_list();
    lexer_set_input(in);
    has_peeked = 0;
}

void close_lexer_stream(void) {
    cleanup_token_list();
}

Token next_token(void) {
    Token t;
    if (has_peeked) {
        has_peeked = 0;
        return peeked_token;
    }

    lex();
    t.type = nextToken;
    t.name = token_name(nextToken);
    t.line = tokenLineNumber;
    strncpy(t.lexeme, lexeme, sizeof(t.lexeme)-1);
    t.lexeme[sizeof(t.lexeme)-1] = '\0';
    return t;
}

Token peek_token(void) {
    if (!has_peeked) {
        peeked_token = next_token();
        has_peeked = 1;
    }
    return peeked_token;
}
