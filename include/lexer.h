#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token_list.h"
#include <string.h>
#include "../include/token_stream.h"
#include "../include/lexer.h"
#include "../include/token_list.h"

int lex(void);
int runLexerTest(int argc, char **argv);
void lexer_set_input(FILE *in);

extern int nextToken;
extern char lexeme[100];
extern int lexLen;
extern int lineNumber;
extern int tokenLineNumber;  /* Line number where current token starts */


extern Token *tokens;
extern size_t count;
extern size_t capacity;

#endif