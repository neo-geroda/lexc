// This will go all the grammar functions
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"  // Include the lookupKeywords function
#include "../include/tokens.h"          // Token definitions
#include "../include/symbol_table.h"    // Symbol table functions
#include "../include/lexer.h"          // Lexer function declarations
#include "../include/token_stream.h"    // Token stream wrapper

// Declare the grammar
void parseProgram();
void parseStatementList(); // this needs to handle 'end' '}'
void parseStatement();
void parseInputStatement();
void parseDataType();
void parseOutputStatement();
void parseExpr();
void parseLit();
void parseAssStatement();
void parseDecStatement();
void parseDecSuffix();

// helper function
void match(int);

// Have the first token to check
Token current_token_parse;

void parse(){
    // current_token = next_token();

    // while(current_token.type != EOF_TOKEN){
    //     parseProgram();
    // }

    current_token_parse = next_token();
    parseProgram();
    match(EOF_TOKEN);

}

// Define the matching function; this consumes the expected lexeme/token
void match(int expected){
    if (current_token_parse.type == expected){
        printf("Consumed: %s", current_token_parse.name);
        current_token_parse = next_token();
    }
    else{
        printf("There is a problem with parsing...");
    }
}

void parseProgram(){
    parseStatementList();
}

void parseStatementList() {
    parseStatement();
    match(SEMICOLON);   // always required

    while (current_token_parse.type != EOF_TOKEN &&
           current_token_parse.type != RCBRACE) {
        parseStatement();
        match(SEMICOLON);
    }
}

void parseStatement(){
    switch(current_token_parse.type){

        // I think this shows state diagram
        case GET: parseInputStatement(); break;
        case DISPLAY: parseOutputStatement(); break;
        case IDENT: parseAssStatement(); break;
        case NUMBER: parseDecStatement(); break;
        case DECIMAL: parseDecStatement(); break;
        case TEXT: parseDecStatement(); break;
        case BOOL: parseDecStatement(); break;
        case SYMBOL: parseDecStatement(); break;
        case LIST: parseDecStatement(); break;

        default:
            printf("Error with parsing a statement.");
        }
}

void parseInputStatement(){

    // handle datatype here

    match(GET);
    match(LEFT_PAREN);
    parseDataType();
    match(RIGHT_PAREN);
    match(SEMICOLON);
}

void parseDataType(){
    switch(current_token_parse.type){
        case NUMBER: 
        case DECIMAL:
        case BOOL:
        case SYMBOL:
        case TEXT:
        case LIST:
            match(current_token_parse.type);
            break;

        default:
            printf("Expected a data type (number, decimal, text, bool, symbol)");
    }
}

void parseOutputStatement(){
    match(DISPLAY);
    match(LEFT_PAREN);
    parseExpr();
    match(RIGHT_PAREN);
    match(SEMICOLON);
}

void parseExpr(){
    if (current_token_parse.type == IDENT){
        match(current_token_parse.type);
        return;
    }
    parseLit();
}

void parseLit(){
    switch(current_token_parse.type){
        case NUM_LIT:
        case DEC_LIT:
        case SYM_LIT:
        case TRUE:
        case FALSE:
            match(current_token_parse.type);
            break;
        
        default:
            printf("Found error in literals");
    }
}

void parseAssStatement(){
    match(IDENT);
    match(EQUALITY_OP);

    // decide which branch based on next token
    if (current_token_parse.type == GET) {
        parseInputStatement(); // id = get(<DATA_TYPE>);
    } else {
        parseExpr();           // id = <EXPR>;
        match(SEMICOLON);      // don't forget semicolon for expressions
    }
}

void parseDecStatement(){
    parseDataType();
    match(IDENT);
    parseDecSuffix();
}

void parseDecSuffix(){
    if (current_token_parse.type == SEMICOLON) {
        match(SEMICOLON);
    }
    else if (current_token_parse.type == EQUALITY_OP) {
        match(EQUALITY_OP);
        parseExpr();
        match(SEMICOLON);
    }
    else {
        printf("Syntax error in declaration\n");
    }
}
