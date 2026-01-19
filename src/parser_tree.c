#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"
#include "../include/symbol_table.h"
#include "../include/lexer.h"
#include "../include/token_stream.h"
#include "../include/token_list.h"


void tree_parseProgram();
void tree_parseStatementList();
void tree_parseStatement();
void tree_parseIdList();
void tree_parseIdListTail();
int tree_parseDataType();
void tree_parseDecStmnt();
void tree_parseDecList();
void tree_parseInputStmnt();
void tree_parseOutputStmnt();
void tree_parseAssStmnt();
void tree_parseConditionalStmnt();
void tree_parseIterStmnt();
void tree_parseDecItem();
void tree_parseDecListTail();
void tree_parseDecItemSuffix();
void tree_parseCompoundStatement();

void tree_parseSimpleStatement();

// void parseRepeat();
void tree_parseJumpStmnt();
void tree_parseExpr();
void tree_parseElifList();
void tree_parseElseOpt();
// void panicRecovery();
// ---- Expression Forward Declarations ----
void tree_parseBoolExpr();
void tree_parseRelExpr();
void tree_parseAddExpr();
void tree_parseTerm();
void tree_parsePower();
void tree_parseUnary();
void tree_parsePostfix();
void tree_parsePrimary();


// tree_match returns 1 (success) or 0 (failed)
int tree_match(int expected);

// Parse index
size_t tree_parse_index = 0;

#define current_token_parse tokens[tree_parse_index]

// --------- PARSE ENTRY POINT ---------

void tree_parse() {
    tree_parse_index = 0;
    tree_parseProgram();
    tree_match(EOF_TOKEN);
}


// This can be edited to just return error codes
int tree_match(int expected) {
    if (current_token_parse.type == expected) {
        // printf("Consumed: %s\n", current_token_parse.name);
        tree_parse_index++;
        return 1;
    }

    // printSyntaxError(expected, current_token_parse.type, current_token_parse.line);
    // panicRecovery();
    return 0;
}

// --------- GRAMMAR ---------
void tree_parseProgram() {
    printf("(LEXC_PROGRAM ");  
    tree_parseStatementList();
    printf(")");               
}

void tree_parseStatementList() {
    printf("(STMNT_LIST ");    // <--- START
    while (tree_parse_index < count &&
           current_token_parse.type != EOF_TOKEN &&
           current_token_parse.type != RCBRACE)
    {
        tree_parseStatement();
        if (current_token_parse.type == SEMICOLON) {
            tree_match(SEMICOLON);
        }
    }
    printf(")");               // <--- END
}

// -------- All about identifiers ------

void tree_parseIdList(){
    if (!tree_match(IDENT)) return;
    tree_parseIdListTail();
}

void tree_parseIdListTail(){    
    if(current_token_parse.type == COMMA){
        if (!tree_match(COMMA)) return;
        if (!tree_match(IDENT)) return;
        tree_parseIdListTail();
    }
}

// ------- Statements enrty point --------

void tree_parseStatement() {
    printf("(STMNT "); // <--- START
    switch (current_token_parse.type) {
        case IF:
        case REPEAT:
        case LCBRACE:     
            tree_parseCompoundStatement();
            break;
        default:
            tree_parseSimpleStatement();
            if (!tree_match(SEMICOLON)) return;
            break;
    }
    printf(")"); // <--- END
}

void tree_parseCompoundStatement() {
    if (current_token_parse.type == IF) {
        tree_parseConditionalStmnt();  // handles if (...) { ... } [elif...] [else...]
        return;
    }
    if (current_token_parse.type == REPEAT) {
        tree_parseIterStmnt();         // handles repeat(...) { ... }
        return;
    }

}

void tree_parseSimpleStatement() {
    switch (current_token_parse.type) {
        case NUMBER: case DECIMAL: case TEXT: case BOOL: case SYMBOL: case LIST:
            tree_parseDecStmnt();    // declaration (no semicolon here)
            break;
        case IDENT:
            tree_parseAssStmnt();    // assignment or expression start
            break;
        case DISPLAY:
            tree_parseOutputStmnt();
            break;
        case STOP: case CONTINUE:
            tree_parseJumpStmnt();
            break;
        default:
            // printf("Syntax error (Line %d): unexpected token %s\n", current_token_parse.line, current_token_parse.name);
            // panicRecovery();
            break;
    }
}

// ---- Declaration Statement ---------

void tree_parseDecStmnt(){
    printf("(DECLARATION "); 
    tree_parseDataType();
    tree_parseDecList();
    printf(")"); 
}

void tree_parseDecList(){
    tree_parseDecItem();
    tree_parseDecListTail();
}

void tree_parseDecListTail(){
    if (current_token_parse.type == COMMA){
        if(!tree_match(COMMA)) return;
        tree_parseDecItem();
        tree_parseDecListTail();
    }
}

void tree_parseDecItem(){
    if(!tree_match(IDENT)) return;
    tree_parseDecItemSuffix();
}

void tree_parseDecItemSuffix(){
    if (current_token_parse.type == ASSIGN_OP){
        if(!tree_match(ASSIGN_OP)) return;

        // special-case: allow `get(...)` directly in declarations
        if (current_token_parse.type == GET) {
            tree_parseInputStmnt();
            return;
        }

        // otherwise tree_parse a normal expression
        tree_parseExpr();
    }
}


// --------- INPUT STATEMENT ---------

void tree_parseInputStmnt() {
    if (!tree_match(GET)) return;
    if (!tree_match(LEFT_PAREN)) return;

    if (!tree_parseDataType()) return;

    if (!tree_match(RIGHT_PAREN)) return;
}

// --------- DATATYPE ---------

int tree_parseDataType() {
    switch (current_token_parse.type) {
        case NUMBER:
        case DECIMAL:
        case BOOL:
        case SYMBOL:
        case TEXT:
        case LIST:
            tree_match(current_token_parse.type);
            return 1;

        default:
            printf("Syntax error (Line %d): expected a data type, found %s\n",
                   current_token_parse.line, current_token_parse.name);
            return 0;
    }
}

// ----- OUTPUT ------

void tree_parseOutputStmnt() {
    printf("(OUTPUT ");
    if (!tree_match(DISPLAY)) return;
    if (!tree_match(LEFT_PAREN)) return;
    tree_parseExpr();
    if (!tree_match(RIGHT_PAREN)) return;
    printf(")");
}

// -----  ASSIGNMENT -------

void tree_parseAssStmnt() {
    printf("(ASSIGNMENT ");
    if (!tree_match(IDENT)) return;
    if (!tree_match(ASSIGN_OP)) return;

    if (current_token_parse.type == GET) {
        tree_parseInputStmnt();  
    } else {
        tree_parseExpr();
    }
    printf(")");
}

// ----------- CONDITIONAL ------------

void tree_parseConditionalStmnt(){
    printf("(CONDITIONAL_STMNT "); // <--- START

    if (!tree_match(IF)) return;
    if (!tree_match(LEFT_PAREN)) return;
    
    // This will print (EXPR ...)
    tree_parseExpr(); 
    
    if (!tree_match(RIGHT_PAREN)) return;
    if (!tree_match(LCBRACE)) return;
    
    // We tree_parse the body
    tree_parseStatementList();
    
    if (!tree_match(RCBRACE)) return;

    tree_parseElifList();
    tree_parseElseOpt();
    
    printf(")"); // <--- END
}

void tree_parseElifList(){
    // tree_parse zero or more `elif (cond) { ... }`
    while (current_token_parse.type == ELIF) {
        if (!tree_match(ELIF)) return;
        if (!tree_match(LEFT_PAREN)) return;
        tree_parseExpr();
        if (!tree_match(RIGHT_PAREN)) return;
        if (!tree_match(LCBRACE)) return;
        tree_parseStatementList();
        if (!tree_match(RCBRACE)) return;
    }
}

void tree_parseElseOpt(){
    if (current_token_parse.type == ELSE){
        if (!tree_match(ELSE)) return;
        if (!tree_match(LCBRACE)) return;
        tree_parseStatementList();
        if (!tree_match(RCBRACE)) return;
    }
}


// ------ ITERATIVE --------
void tree_parseIterStmnt(){
    if (!tree_match(REPEAT)) return;
    if (!tree_match(LEFT_PAREN)) return;
    tree_parseExpr();
    if (!tree_match(RIGHT_PAREN)) return;
    if (!tree_match(LCBRACE)) return;
    tree_parseStatementList();
    if (!tree_match(RCBRACE)) return;

}

void tree_parseJumpStmnt(){
    switch(current_token_parse.type){
        case CONTINUE:
            if(!tree_match(CONTINUE)) return;
            break;
        case STOP:
            if (!tree_match(STOP)) return;
            break;
    }
}

// ----- All about Expression -------

void tree_parseExpr () {
    printf("(EXPR ");
    tree_parseBoolExpr ();
    printf(")");
}

void tree_parseBoolExpr () {
    tree_parseRelExpr ();

        while (current_token_parse.type == OR_OP)
        {
            tree_match(OR_OP);
            tree_parseRelExpr();
        }
}

void tree_parseRelExpr (){
    tree_parseAddExpr();

    switch (current_token_parse.type)
    {
    case EQUALITY_OP:
    case INEQUALITY_OP:
    case GREATER_THAN_OP:
    case LESS_THAN_OP:
    case GREATER_EQUAL_OP:
    case LESS_EQUAL_OP:
        tree_match(current_token_parse.type);
        tree_parseAddExpr();
    break;
    
    default:
        break;
    }
}

void tree_parseAddExpr (){
        tree_parseTerm();
            
            while(current_token_parse.type == ADDITION_OP ||
                    current_token_parse.type == SUBTRACT_OP){
                        tree_match(current_token_parse.type);
                        tree_parseTerm();
                    }
}

void tree_parseTerm(){
    tree_parsePower();

        while(current_token_parse.type == MULTIPLY_OP ||
        current_token_parse.type == DIVIDE_OP ||
        current_token_parse.type == FLOORDIV_OP ||
        current_token_parse.type == MODULO_OP) {
            tree_match(current_token_parse.type);
            tree_parsePower();
        }
}

void tree_parsePower() {
    tree_parseUnary();

        while(current_token_parse.type == EXPONENT_OP){
            tree_match(EXPONENT_OP);
            tree_parseUnary();
        }
}

void tree_parseUnary() {

        while(current_token_parse.type == ADDITION_OP ||
            current_token_parse.type == SUBTRACT_OP ||
            current_token_parse.type == PRE_INCREMENT_OP ||
            current_token_parse.type == PRE_DECREMENT_OP ||
            current_token_parse.type == NOT_OP){
                tree_match(current_token_parse.type);
                tree_parseUnary();
            }
            tree_parsePostfix();
}

void tree_parsePostfix(){
    tree_parsePrimary();

    while(current_token_parse.type == POST_INCREMENT_OP ||
        current_token_parse.type == POST_DECREMENT_OP) {
            tree_match(current_token_parse.type);
        }
        
}

void tree_parsePrimary(){
    printf("(PRIMARY ");

    switch(current_token_parse.type) {
        case NUM_LIT:
        case DEC_LIT:
        case SYM_LIT:
        case TRUE:
        case FALSE:
        case TEXT_LIT:
            // This prints the actual value, e.g., (PRIMARY VAL:5) or (PRIMARY VAL:"hello")
            printf("VAL:%s ", current_token_parse.lexeme); 
            tree_match(current_token_parse.type);
            break;

        case IDENT:
            // This prints the variable name, e.g., (PRIMARY ID:x)
            printf("ID:%s ", current_token_parse.lexeme);
            tree_match(IDENT);
            break;

        case LEFT_PAREN:
            // Parentheses don't need values printed, just tree_match them
            tree_match(LEFT_PAREN);
            tree_parseExpr();
            tree_match(RIGHT_PAREN);
            break;

        default:
            // printf("Syntax error: expected literal, found %s\n",
            //        current_token_parse.name);
            // panicRecovery();
            break;
    }
    printf(")");
}