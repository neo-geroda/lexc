#include <stdio.h>
#include "tokens.h"

// Declare lex() and nextToken from lexer.c
extern int lex();
extern int nextToken;

// Forward declarations for grammar rules
void parse();
void assignment();
void statement();
void expression();

// Entry point for the parser
void parse() {
    lex(); // Get the first token
    while (nextToken != EOF_TOKEN) {
        statement();
    }
}

// Example: statement can be an assignment (expand as needed)
void statement() {
    if (nextToken == IDENT) {
        assignment();
    } else {
        printf("Syntax Error: Unexpected token %s\n", token(nextToken));
        lex();
    }
}

// assignment → IDENT ASSIGN_OP expression SEMICOLON
void assignment() {
    if (nextToken == IDENT) {
        lex();
        if (nextToken == ASSIGN_OP) {
            lex();
            expression();
            if (nextToken == SEMICOLON) {
                printf("Valid assignment statement.\n");
                lex();
            } else {
                printf("Syntax Error: Missing semicolon after assignment.\n");
                lex();
            }
        } else {
            printf("Syntax Error: Missing assignment operator.\n");
            lex();
        }
    } else {
        printf("Syntax Error: Assignment must start with identifier.\n");
        lex();
    }
}

// expression → NUM_LIT | DEC_LIT | IDENT (expand as needed)
void expression() {
    if (nextToken == NUM_LIT) {
        printf("Assignment: variable = NUMERIC LITERAL\n");
        lex();
    } else if (nextToken == DEC_LIT) {
        printf("Assignment: variable = DECIMAL LITERAL\n");
        lex();
    } else if (nextToken == IDENT) {
        printf("Assignment: variable = variable\n");
        lex();
    } else {
        printf("Syntax Error: Invalid expression.\n");
        lex();
    }
}

// Example main for parser
int main() {
    parse();
    return 0;
}
