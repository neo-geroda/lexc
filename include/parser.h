#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "tokens.h"          // for token types
#include "lexer.h"           // for next_token()
#include "token_stream.h"    // if needed for the stream
#include "symbol_table.h"    // if parser interacts with symbol table

extern Token current_token_parse;

// Expose the main parse() function
void parse();

// Expose your grammar functions if needed externally
void parseProgram();
void parseStatementList();
void parseStatement();
void parseInputStatement();
void parseDataType();
void parseOutputStatement();
void parseExpr();
void parseLit();
void parseAssStatement();
void parseDecStatement();
void parseDecSuffix();

// Optionally expose match() if other modules need it
void match(int expected);

// Optionally expose current_token_parse, if other modules access it
extern Token current_token_parse;

#endif // PARSER_H
