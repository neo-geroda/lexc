#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"
#include "../include/symbol_table.h"
#include "../include/lexer.h"
#include "../include/token_stream.h"

// ---- Forward declarations ----
void parseProgram();
void parseStatementList();
void parseStatement();
void parseInputStatement();
int  parseDataType();
void parseOutputStatement();
void parseExpr();
void parseLit();
void parseAssStatement();
void parseDecStatement();
void parseDecSuffix();
void panicRecovery();

// match returns 1 (success) or 0 (failed)
int match(int expected);

// Parse index
size_t parse_index = 0;

#define current_token_parse tokens[parse_index]

// --------- PARSE ENTRY POINT ---------

void parse() {
    parse_index = 0;
    parseProgram();
    match(EOF_TOKEN);
}

// --------- MATCH & RECOVERY ---------

int match(int expected) {
    if (current_token_parse.type == expected) {
        printf("Consumed: %s\n", current_token_parse.name);
        parse_index++;
        return 1;
    }

    printf("Syntax error: expected token %d but found %s\n",
           expected, current_token_parse.name);

    panicRecovery();
    return 0;
}

void panicRecovery() {
    printf("Entering panic recovery...\n");

    while (parse_index < count &&
           current_token_parse.type != SEMICOLON &&
           current_token_parse.type != RCBRACE &&
           current_token_parse.type != EOF_TOKEN)
    {
        printf("Skipping token: %s\n", current_token_parse.name);
        parse_index++;
    }

    if (current_token_parse.type == SEMICOLON) {
        printf("Recovered at ;\n");
        parse_index++;
    }
}

// --------- GRAMMAR ---------

void parseProgramTail1() {
    while (current_token_parse.type != EOF_TOKEN) {
        if (isFunctionDefAhead()) {
            parseFunctionDef();
        } else {
            parseStatement();
            match(SEMICOLON);
        }
    }
}

void parseFunctionDef() {
    match(IDENT);        // function name
    match(LEFT_PAREN);   // '('

    // Optional parameter list
    if (current_token_parse.type == IDENT) {
        parseIDList();
    }

    match(RIGHT_PAREN);  // ')'
    match(LCBRACE);      // '{'

    parseStatementList(); // function body

    match(RCBRACE);      // '}'
}

void parseIDList() {
    match(IDENT);        // first parameter
    while (current_token_parse.type == COMMA) {
        match(COMMA);
        match(IDENT);    // next parameter
    }
}


void parseProgram() {
    parseStatementList();
}

void parseStatementList() {
    while (parse_index < count &&
           current_token_parse.type != EOF_TOKEN &&
           current_token_parse.type != RCBRACE)
    {
        parseStatement();
    }
}

void parseStatement() {
    switch (current_token_parse.type) {

        case GET:
            parseInputStatement();
            break;

        case DISPLAY:
            parseOutputStatement();
            break;

        case IDENT:
            parseAssStatement();
            break;

        case NUMBER:
        case DECIMAL:
        case TEXT:
        case BOOL:
        case SYMBOL:
        case LIST:
            parseDecStatement();
            break;

        default:
            printf("Syntax error: unexpected token %s\n",
                   current_token_parse.name);
            panicRecovery();
            break;
    }
}

// --------- INPUT STATEMENT ---------

void parseInputStatement() {
    if (!match(GET)) return;
    if (!match(LEFT_PAREN)) return;

    if (!parseDataType()) return;

    if (!match(RIGHT_PAREN)) return;
    if (!match(SEMICOLON)) return;
}

// --------- DATATYPE ---------

int parseDataType() {
    switch (current_token_parse.type) {
        case NUMBER:
        case DECIMAL:
        case BOOL:
        case SYMBOL:
        case TEXT:
        case LIST:
            match(current_token_parse.type);
            return 1;

        default:
            printf("Syntax error: expected a data type, found %s\n",
                   current_token_parse.name);
            return 0;
    }
}

// --------- OUTPUT STATEMENT ---------

void parseOutputStatement() {
    if (!match(DISPLAY)) return;
    if (!match(LEFT_PAREN)) return;

    parseExpr();

    if (!match(RIGHT_PAREN)) return;
    if (!match(SEMICOLON)) return;
}

// --------- EXPRESSIONS ---------

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
            printf("Syntax error: expected literal, found %s\n",
                   current_token_parse.name);
            break;
    }
}

// --------- ASSIGNMENT ---------

void parseAssStatement() {
    if (!match(IDENT)) return;
    if (!match(ASSIGN_OP)) return;

    if (current_token_parse.type == GET) {
        parseInputStatement();  // already matched semicolon inside
        return;
    }

    parseExpr();
    match(SEMICOLON); // safe, will recover if wrong
}

// --------- DECLARATION ---------

void parseDecStatement() {
    if (!parseDataType()) return;
    if (!match(IDENT)) return;

    parseDecSuffix();
}

void parseDecSuffix() {
    if (current_token_parse.type == SEMICOLON) {
        match(SEMICOLON);
    }
    else if (current_token_parse.type == ASSIGN_OP) {
        match(ASSIGN_OP);
        parseExpr();
        match(SEMICOLON);
    }
    else {
        printf("Syntax error in declaration at token %s\n",
               current_token_parse.name);
        panicRecovery();
    }
}
