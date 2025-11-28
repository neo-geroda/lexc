#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <stdio.h>

typedef struct {
    int type;
    const char *name;
    char lexeme[128];
    int line;
} Token;

void init_lexer_stream(FILE *in);

void close_lexer_stream(void);

Token next_token(void);

Token peek_token(void);

#endif /* TOKEN_STREAM_H */
