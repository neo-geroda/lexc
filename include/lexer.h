#ifndef LEXER_H
#define LEXER_H

#include <stdio.h> 

int lex(void);
int runLexerTest(int argc, char **argv);

extern int nextToken;
extern char lexeme[100];
extern int lexLen;
extern int lineNumber;

#endif