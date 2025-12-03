#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"
#include "../include/symbol_table.h"
#include "../include/lexer.h"
#include "../include/token_stream.h"
#include "../include/token_list.h"

// void parseLexC();
// void parseProgramItem();
// void parseProgramTail();
// void parseFunctionDef();
// void parseOptParamDefs();

void parseProgram();
void parseStatementList();
void parseStatement();
void parseIdList();
void parseIdListTail();
int parseDataType();
void parseDecStmnt();
void parseDecList();
void parseInputStmnt();
void parseOutputStmnt();
void parseAssStmnt();
void parseConditionalStmnt();
void parseIterStmnt();
void parseDecItem();
void parseDecListTail();
void parseDecItemSuffix();

// void parseRepeat();
void parseJumpStmnt();
void parseDecSuffix();
void parseExpr();
void parseElifList();
void parseElseOpt();
void panicRecovery();
// ---- Expression Forward Declarations ----
void parseBoolExpr();
void parseRelExpr();
void parseAddExpr();
void parseTerm();
void parsePower();
void parseUnary();
void parsePostfix();
void parsePrimary();


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

// This can be edited to just return error codes
int match(int expected) {
    if (current_token_parse.type == expected) {
        printf("Consumed: %s\n", current_token_parse.name);
        parse_index++;
        return 1;
    }

    printf("Syntax error: expected token %d: %s but found %d: %s\n",
           expected, token_name(expected),
           current_token_parse.type, token_name(current_token_parse.type));

    panicRecovery();
    return 0;
}

void panicRecovery() {
    printf("Entering panic recovery...\n");

    while (parse_index < count &&
           current_token_parse.type != SEMICOLON &&
           current_token_parse.type != EOF_TOKEN)
    {
        printf("Skipping token: %s\n", current_token_parse.name);
        parse_index++;
    }

    if (current_token_parse.type == SEMICOLON) {
        printf("Recovered at ;\n");

        // because you are consuming semicolon here and then outside too
        // parse_index++;
    }
}

// --------- GRAMMAR ---------
void parseProgram() {
    parseStatementList();
}

void parseStatementList() {
    while (parse_index < count &&
           current_token_parse.type != EOF_TOKEN &&
        current_token_parse.type != RCBRACE)
    {
        parseStatement();

        if (current_token_parse.type == SEMICOLON) {
            match(SEMICOLON);
        }
    }
}

// -------- All about identifiers ------

void parseIdList(){
    if (!match(IDENT)) return;
    parseIdListTail();
}

void parseIdListTail(){    
    if(current_token_parse.type == COMMA){
        if (!match(COMMA)) return;
        if (!match(IDENT)) return;
        parseIdListTail();
    }
}

// ------- Statements enrty point --------

void parseStatement() {
    switch (current_token_parse.type) {

        case GET:
            parseInputStmnt();
            break;

        case DISPLAY:
            parseOutputStmnt();
            break;

        case IDENT:
            parseAssStmnt();
            break;
        
        case IF:
            parseConditionalStmnt();
            break;
        
        // Also return here
        case STOP:
        case CONTINUE:
            parseJumpStmnt();
            break;

        case NUMBER:
        case DECIMAL:
        case TEXT:
        case BOOL:
        case SYMBOL:
        case LIST:
            parseDecStmnt();
            break;

        case REPEAT:
            parseIterStmnt();
            break;

        default:
            printf("Syntax error: unexpected token %s\n",
                   current_token_parse.name);
            panicRecovery();
            break;
    }
}

// ---- Declaration Statement ---------

void parseDecStmnt(){
    parseDataType();
    // parseIdList();
    parseDecList();
    // if (!match(IDENT)) return;

    // parseDecItemSuffix();
}

void parseDecList(){
    parseDecItem();
    parseDecListTail();
}

void parseDecListTail(){
    if (current_token_parse.type == COMMA){
        if(!match(COMMA)) return;
        parseDecItem();
        parseDecListTail();
    }
}

void parseDecItem(){
    if(!match(IDENT)) return;
    parseDecItemSuffix();
}

void parseDecItemSuffix(){
    if (current_token_parse.type == ASSIGN_OP){
        if(!match(ASSIGN_OP)) return;

        // special-case: allow `get(...)` directly in declarations
        if (current_token_parse.type == GET) {
            parseInputStmnt();
            return;
        }

        // otherwise parse a normal expression
        parseExpr();
    }
}


// --------- INPUT STATEMENT ---------

void parseInputStmnt() {
    if (!match(GET)) return;
    if (!match(LEFT_PAREN)) return;

    if (!parseDataType()) return;

    if (!match(RIGHT_PAREN)) return;
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

// ----- OUTPUT ------

void parseOutputStmnt() {
    if (!match(DISPLAY)) return;
    if (!match(LEFT_PAREN)) return;

    parseExpr();

    if (!match(RIGHT_PAREN)) return;
}

// -----  ASSIGNMENT -------

void parseAssStmnt() {
    if (!match(IDENT)) return;
    if (!match(ASSIGN_OP)) return;

    if (current_token_parse.type == GET) {
        parseInputStmnt();  
        return;
    }

    parseExpr();
}

// ----------- CONDITIONAL ------------

void parseConditionalStmnt(){

    if (!match(IF)) return;
    if (!match(LEFT_PAREN)) return;
    parseExpr();
    if (!match(RIGHT_PAREN)) return;
    if (!match(LCBRACE)) return;
    parseStatementList();
    if (!match(RCBRACE)) return;

    parseElifList();
    parseElseOpt();
}

void parseElifList(){
    // parse zero or more `elif (cond) { ... }`
    while (current_token_parse.type == ELIF) {
        if (!match(ELIF)) return;
        if (!match(LEFT_PAREN)) return;
        parseExpr();
        if (!match(RIGHT_PAREN)) return;
        if (!match(LCBRACE)) return;
        parseStatementList();
        if (!match(RCBRACE)) return;
    }
}

void parseElseOpt(){
    if (current_token_parse.type == ELSE){
        if (!match(ELSE)) return;
        if (!match(LCBRACE)) return;
        parseStatementList();
        if (!match(RCBRACE)) return;
    }
}


// ------ ITERATIVE --------
void parseIterStmnt(){
    if (!match(REPEAT)) return;
    if (!match(LEFT_PAREN)) return;
    parseExpr();
    if (!match(RIGHT_PAREN)) return;
    if (!match(LCBRACE)) return;
    parseStatementList();
    if (!match(RCBRACE)) return;

}

void parseJumpStmnt(){
    switch(current_token_parse.type){
        case CONTINUE:
            if(!match(CONTINUE)) return;
            break;
        case STOP:
            if (!match(STOP)) return;
            break;
    }
}

// ----- All about Expression -------

void parseExpr () {
    parseBoolExpr ();
}

void parseBoolExpr () {
    parseRelExpr ();

        while (current_token_parse.type == OR_OP)
        {
            match(OR_OP);
            parseRelExpr();
        }
}

void parseRelExpr (){
    parseAddExpr();

    switch (current_token_parse.type)
    {
    case EQUALITY_OP:
    case INEQUALITY_OP:
    case GREATER_THAN_OP:
    case LESS_THAN_OP:
    case GREATER_EQUAL_OP:
    case LESS_EQUAL_OP:
        match(current_token_parse.type);
        parseAddExpr();
    break;
    
    default:
        break;
    }
}

void parseAddExpr (){
        parseTerm();
            
            while(current_token_parse.type == ADDITION_OP ||
                    current_token_parse.type == SUBTRACT_OP){
                        match(current_token_parse.type);
                        parseTerm();
                    }
}

void parseTerm(){
    parsePower();

        while(current_token_parse.type == MULTIPLY_OP ||
        current_token_parse.type == DIVIDE_OP ||
        current_token_parse.type == FLOORDIV_OP ||
        current_token_parse.type == MODULO_OP) {
            match(current_token_parse.type);
            parsePower();
        }
}

void parsePower() {
    parseUnary();

        while(current_token_parse.type == EXPONENT_OP){
            match(EXPONENT_OP);
            parseUnary();
        }
}

void parseUnary() {

        while(current_token_parse.type == ADDITION_OP ||
            current_token_parse.type == SUBTRACT_OP ||
            current_token_parse.type == PRE_INCREMENT_OP ||
            current_token_parse.type == PRE_DECREMENT_OP ||
            current_token_parse.type == NOT_OP){
                match(current_token_parse.type);
                parseUnary();
            }
            parsePostfix();
}

void parsePostfix(){
    parsePrimary();

    while(current_token_parse.type == POST_INCREMENT_OP ||
        current_token_parse.type == POST_DECREMENT_OP) {
            match(current_token_parse.type);
        }
        
}

void parsePrimary(){

    switch(current_token_parse.type) {
        case NUM_LIT:
        case DEC_LIT:
        case SYM_LIT:
        case TRUE:
        case FALSE:
        case TEXT_LIT:
            match(current_token_parse.type);
            break;

            case IDENT:
                match(IDENT);
            return;

            case LEFT_PAREN:
                match(LEFT_PAREN);
                parseExpr();
                match(RIGHT_PAREN);
            return;

            default:
            printf("Syntax error: expected literal, found %s\n",
                   current_token_parse.name);
            panicRecovery();
            break;
            } 
}




/*
// ---- Forward declarations ----
// void parseProgram();
// void parseStatementList();
// void parseStatement();
// void parseInputStatement();
// int  parseDataType();
// void parseOutputStatement();
// void parseExpr();
// void parseLit();
// void parseAssStatement();
// void parseDecStatement();
// void parseDecSuffix();
// void panicRecovery();

// void parseProgram() {
//     parseStatementList();
// }

// void parseStatementList() {
//     while (parse_index < count &&
//            current_token_parse.type != EOF_TOKEN &&
//            current_token_parse.type != RCBRACE)
//     {
//         parseStatement();
//     }
// }

// void parseStatement() {
//     switch (current_token_parse.type) {

//         case GET:
//             parseInputStatement();
//             break;

//         case DISPLAY:
//             parseOutputStatement();
//             break;

//         case IDENT:
//             parseAssStatement();
//             break;

//         case NUMBER:
//         case DECIMAL:
//         case TEXT:
//         case BOOL:
//         case SYMBOL:
//         case LIST:
//             parseDecStatement();
//             break;

//         default:
//             printf("Syntax error: unexpected token %s\n",
//                    current_token_parse.name);
//             panicRecovery();
//             break;
//     }
// }

// // --------- INPUT STATEMENT ---------

// void parseInputStatement() {
//     if (!match(GET)) return;
//     if (!match(LEFT_PAREN)) return;

//     if (!parseDataType()) return;

//     if (!match(RIGHT_PAREN)) return;
//     if (!match(SEMICOLON)) return;
// }

// // --------- DATATYPE ---------

// int parseDataType() {
//     switch (current_token_parse.type) {
//         case NUMBER:
//         case DECIMAL:
//         case BOOL:
//         case SYMBOL:
//         case TEXT:
//         case LIST:
//             match(current_token_parse.type);
//             return 1;

//         default:
//             printf("Syntax error: expected a data type, found %s\n",
//                    current_token_parse.name);
//             return 0;
//     }
// }

// // --------- OUTPUT STATEMENT ---------

// void parseOutputStatement() {
//     if (!match(DISPLAY)) return;
//     if (!match(LEFT_PAREN)) return;

//     parseExpr();

//     if (!match(RIGHT_PAREN)) return;
//     if (!match(SEMICOLON)) return;
// }

// // --------- EXPRESSIONS ---------

// void parseExpr() {
//     if (current_token_parse.type == IDENT) {
//         match(IDENT);
//         return;
//     }
//     parseLit();
// }

// void parseLit() {
//     switch (current_token_parse.type) {
//         case NUM_LIT:
//         case DEC_LIT:
//         case SYM_LIT:
//         case TRUE:
//         case FALSE:
//             match(current_token_parse.type);
//             break;

//         default:
//             printf("Syntax error: expected literal, found %s\n",
//                    current_token_parse.name);
//             break;
//     }
// }

// // --------- ASSIGNMENT ---------

// void parseAssStatement() {
//     if (!match(IDENT)) return;
//     if (!match(ASSIGN_OP)) return;

//     if (current_token_parse.type == GET) {
//         parseInputStatement();  // already matched semicolon inside
//         return;
//     }

//     parseExpr();
//     match(SEMICOLON); // safe, will recover if wrong
// }

// // --------- DECLARATION ---------

// void parseDecStatement() {
//     if (!parseDataType()) return;
//     if (!match(IDENT)) return;

//     parseDecSuffix();
// }

// void parseDecSuffix() {
//     if (current_token_parse.type == SEMICOLON) {
//         match(SEMICOLON);
//     }
//     else if (current_token_parse.type == ASSIGN_OP) {
//         match(ASSIGN_OP);
//         parseExpr();
//         match(SEMICOLON);
//     }
//     else {
//         printf("Syntax error in declaration at token %s\n",
//                current_token_parse.name);
//         panicRecovery();
//     }
// }
*/