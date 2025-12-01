#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"
#include "../include/symbol_table.h"
#include "../include/lexer.h"
#include "../include/token_stream.h"

// Grammar function declarations
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

// Helper function
void match(int);

// Current token index
size_t parse_index = 0;

// Macro to get current token
#define current_token_parse tokens[parse_index]

// Parser entry point
void parse() {
    parse_index = 0; // start at first token
    parseProgram();
    match(EOF_TOKEN); // ensure we end on EOF
}

// Match function consumes the expected token
void match(int expected){
    if (current_token_parse.type == expected){
        printf("Consumed: %s\n", current_token_parse.name);
        parse_index++; // move to next token
    }
    else{
        printf("Syntax error: expected token %d but found %s\n",
               expected, current_token_parse.name);
        parse_index++; //advance to prevent infinite loop
    }
}

// ----- Grammar functions -----

void parseProgram() {
    parseStatementList();
}

void parseStatementList() {
    while (parse_index < count &&
           current_token_parse.type != EOF_TOKEN &&
           current_token_parse.type != RCBRACE) {
        parseStatement();
        // no match(SEMICOLON) here — statements handle their own
    }
}


void parseStatement() {
    switch (current_token_parse.type) {
        case GET: parseInputStatement(); break;
        case DISPLAY: parseOutputStatement(); break;
        case IDENT: parseAssStatement(); break;
        case NUMBER:
        case DECIMAL:
        case TEXT:
        case BOOL:
        case SYMBOL:
        case LIST: parseDecStatement(); break;
        default:
            printf("Syntax error: unexpected token %s\n", current_token_parse.name);
    }
}

void parseInputStatement() {
    match(GET);
    match(LEFT_PAREN);
    parseDataType();
    match(RIGHT_PAREN);
    match(SEMICOLON);
}

void parseDataType() {
    switch (current_token_parse.type) {
        case NUMBER:
        case DECIMAL:
        case BOOL:
        case SYMBOL:
        case TEXT:
        case LIST:
            match(current_token_parse.type);
            break;
        default:
            printf("Syntax error: expected a data type, found %s\n", current_token_parse.name);
    }
}

void parseOutputStatement() {
    match(DISPLAY);
    match(LEFT_PAREN);
    parseExpr();
    match(RIGHT_PAREN);
    match(SEMICOLON);
}

void parseExpr() {
    if (current_token_parse.type == IDENT) {
        match(IDENT);
        return;
    }
    parseLit();
}

void parseLit() {
    switch (current_token_parse.type) {
        case NUM_LIT:
        case DEC_LIT:
        case SYM_LIT:
        case TRUE:
        case FALSE:
            match(current_token_parse.type);
            break;
        default:
            printf("Syntax error: expected literal, found %s\n", current_token_parse.name);
    }
}

void parseAssStatement() {
    match(IDENT);
    match(ASSIGN_OP);

    if (current_token_parse.type == GET) {
        parseInputStatement(); // id = get(<DATA_TYPE>);
    } else {
        parseExpr();           // id = <EXPR>;
        match(SEMICOLON);
    }
}

void parseDecStatement() {
    parseDataType();
    match(IDENT);
    parseDecSuffix();
}

void parseDecSuffix() {
    if (current_token_parse.type == SEMICOLON) {
        match(SEMICOLON);
    } else if (current_token_parse.type == ASSIGN_OP) {
        match(ASSIGN_OP);
        parseExpr();
        match(SEMICOLON);
    } else {
        printf("Syntax error in declaration at token %s\n", current_token_parse.name);
    }
}
