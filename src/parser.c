#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/lookupkeywords.h"
#include "../include/tokens.h"
#include "../include/symbol_table.h"
#include "../include/lexer.h"
#include "../include/token_stream.h"
#include "../include/token_list.h"
#include "../include/parser.h"

// --------- AST Node Implementation ---------

ASTNode* createNode(NodeType type, const char* value, int token_type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->token_type = token_type;
    node->children = NULL;
    node->child_count = 0;
    node->child_capacity = 0;
    return node;
}

void addChild(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity = parent->child_capacity == 0 ? 4 : parent->child_capacity * 2;
        parent->children = (ASTNode**)realloc(parent->children, 
                                              parent->child_capacity * sizeof(ASTNode*));
    }
    parent->children[parent->child_count++] = child;
}

void freeAST(ASTNode* node) {
    if (!node) return;
    
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    
    free(node->children);
    free(node->value);
    free(node);
}

const char* nodeTypeToString(NodeType type) {
    switch(type) {
        // Program Structure
        case NODE_LEXC_PROGRAM: return "<LexCProgram>";
        case NODE_STMNT_LIST: return "<STMNT_LIST>";
        
        // Statements
        case NODE_SIMPLE_STMNT: return "<SIMPLE_STMNT>";
        case NODE_COMPOUND_STMNT: return "<COMPOUND_STMNT>";
        case NODE_DECLARATION_STMNT: return "<DECLARATION_STMNT>";
        case NODE_ASSMNT_STMNT: return "<ASSMNT_STMNT>";
        case NODE_INPUT_STMNT: return "<INPUT_STMNT>";
        case NODE_OUTPUT_STMNT: return "<OUTPUT_STMNT>";
        case NODE_JUMP_STMNT: return "<JUMP_STMNT>";
        case NODE_CONDITIONAL_STMNT: return "<CONDITIONAL_STMNT>";
        case NODE_ITERATIVE_STMNT: return "<ITERATIVE_STMNT>";
        case NODE_REPEAT: return "<REPEAT>";
        case NODE_REPEAT_FOR: return "<REPEAT_FOR>";
        
        // Expression Hierarchy
        case NODE_EXPR: return "<EXPR>";
        case NODE_BOOL_EXPR: return "<BOOL_EXPR>";
        case NODE_AND_TERM: return "<AND_TERM>";
        case NODE_NOT_TERM: return "<NOT_TERM>";
        case NODE_REL_EXPR: return "<REL_EXPR>";
        case NODE_ADDITIVE_EXPR: return "<ADDITIVE_EXPR>";
        case NODE_TERM: return "<TERM>";
        case NODE_POWER: return "<POWER>";
        case NODE_UNARY_EXPR: return "<UNARY_EXPR>";
        case NODE_POSTFIX_EXPR: return "<POSTFIX_EXPR>";
        case NODE_PRIMARY: return "<PRIMARY>";
        
        // Components
        case NODE_DATA_TYPE: return "<DATA_TYPE>";
        case NODE_DECL_LIST: return "<DECL_LIST>";
        case NODE_DECL_ITEM: return "<DECL_ITEM>";
        case NODE_IDENTIFIER: return "id";
        case NODE_LITERAL: return "literal";
        
        // Operators
        case NODE_ADDITIVE_OP: return "<ADDITIVE_OP>";
        case NODE_MULTIPLICATIVE_OP: return "<MULTIPLICATIVE_OP>";
        case NODE_COMPARE_OP: return "<COMPARE_OP>";
        case NODE_LOGICAL_OP: return "<LOGICAL_OP>";
        
        // Control Flow Components
        case NODE_ELIF_CLAUSE: return "elif";
        case NODE_ELSE_CLAUSE: return "else";
        
        default: return "<UNKNOWN>";
    }
}

void printAST(ASTNode* node, int indent) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
    
    // Print node type
    printf("%s", nodeTypeToString(node->type));
    
    // Print value if present
    if (node->value) {
        printf("(%s)", node->value);
    }
    
    printf("\n");
    
    // Print children
    for (int i = 0; i < node->child_count; i++) {
        printAST(node->children[i], indent + 1);
    }
}

// void parseLexC();
// void parseProgramItem();
// void parseProgramTail();
// void parseFunctionDef();
// void parseOptParamDefs();

ASTNode* parseProgram();
ASTNode* parseStatementList();
ASTNode* parseStatement();
void parseIdList();
void parseIdListTail();
int parseDataType();
ASTNode* parseDecStmnt();
void parseDecList(ASTNode* parent);
ASTNode* parseInputStmnt();
ASTNode* parseOutputStmnt();
ASTNode* parseAssStmnt();
ASTNode* parseConditionalStmnt();
ASTNode* parseIterStmnt();
ASTNode* parseDecItem(ASTNode* parent);
void parseDecListTail(ASTNode* parent);
void parseDecItemSuffix(ASTNode* parent);
ASTNode* parseCompoundStatement();

ASTNode* parseSimpleStatement();

// void parseRepeat();
void parseJumpStmnt(ASTNode* parent);
void parseDecSuffix();
ASTNode* parseExpr();
void parseElifList(ASTNode* parent);
void parseElseOpt(ASTNode* parent);
void panicRecovery();
// ---- Expression Forward Declarations ----
ASTNode* parseBoolExpr();
ASTNode* parseRelExpr();
ASTNode* parseAddExpr();
ASTNode* parseTerm();
ASTNode* parsePower();
ASTNode* parseUnary();
ASTNode* parsePostfix();
ASTNode* parsePrimary();


// match returns 1 (success) or 0 (failed)
int match(int expected);

// Parse index
size_t parse_index = 0;

#define current_token_parse tokens[parse_index]

// --------- PARSE ENTRY POINT ---------

ASTNode* parse() {
    parse_index = 0;
    ASTNode* root = parseProgram();
    match(EOF_TOKEN);
    return root;
}

// --------- MATCH & RECOVERY ---------

// Reliable token name mapping using switch statement
// This doesn't depend on token_list initialization
const char* tokenTypeToString(int t) {
    switch(t) {
        // Basic tokens
        case IDENT: return "IDENTIFIER";
        case ASSIGN_OP: return "=";
        case POS_OP: return "POS_OP";
        case NEG_OP: return "NEG_OP";
        case LEFT_PAREN: return "LEFT_PAREN (";
        case RIGHT_PAREN: return "RIGHT_PAREN )";
        
        // Arithmetic Operators
        case ADDITION_OP: return "+";
        case SUBTRACT_OP: return "-";
        case MULTIPLY_OP: return "*";
        case DIVIDE_OP: return "/";
        case MODULO_OP: return "%";
        case EXPONENT_OP: return "**";
        case FLOORDIV_OP: return "//";
        case PRE_INCREMENT_OP: return "++";
        case POST_INCREMENT_OP: return "++";
        case PRE_DECREMENT_OP: return "--";
        case POST_DECREMENT_OP: return "--";
        
        // Relational Operators
        case EQUALITY_OP: return "==";
        case INEQUALITY_OP: return "!=";
        case GREATER_THAN_OP: return ">";
        case LESS_THAN_OP: return "<";
        case GREATER_EQUAL_OP: return ">=";
        case LESS_EQUAL_OP: return "<=";
        
        // Logical Operators
        case AND_OP: return "AND";
        case OR_OP: return "OR";
        case NOT_OP: return "NOT";
        
        // Keywords
        case IF: return "IF";
        case ELIF: return "ELIF";
        case ELSE: return "ELSE";
        case REPEAT: return "REPEAT";
        case REPEATFOR: return "REPEATFOR";
        case STOP: return "STOP";
        case CONTINUE: return "CONTINUE";
        case DISPLAY: return "DISPLAY";
        case GET: return "GET";
        case GIVE: return "GIVE";
        case FUNCTION: return "FUNCTION";
        case STATIC: return "STATIC";
        case INCLUDE: return "INCLUDE";
        case TRY: return "TRY";
        case BUNDLE: return "BUNDLE";
        case VOID: return "VOID";
        case IN: return "IN";
        case ONFAIL: return "ONFAIL";
        
        // Data Types
        case NUMBER: return "NUMBER";
        case DECIMAL: return "DECIMAL";
        case SYMBOL: return "SYMBOL";
        case BOOL: return "BOOL";
        case STRING: return "STRING";
        case TEXT: return "TEXT";
        case LIST: return "LIST";
        
        // Literals
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case NOTHING: return "NOTHING";
        case NUM_LIT: return "NUMBER_LITERAL";
        case DEC_LIT: return "DECIMAL_LITERAL";
        case TEXT_LIT: return "TEXT_LITERAL";
        case SYM_LIT: return "SYMBOL_LITERAL";
        
        // Delimiters
        case SEMICOLON: return ";";
        case COMMA: return ",";
        case LSQBRACKET: return "[";
        case RSQBRACKET: return "]";
        case LCBRACE: return "LEFT_BRACE {";
        case RCBRACE: return "RIGHT_BRACE }";
        case DQUOTE: return "\"";
        case SQUOTE: return "'";
        case COLON: return ":";
        
        // Noise Words
        case NOISE_WORD: return "NOISE_WORD";
        
        // Comments
        case SINGLE_LINE_COMMENT: return "SINGLE_LINE_COMMENT";
        case MULTILINE_COMMENT: return "MULTILINE_COMMENT";
        
        // Special
        case EOF_TOKEN: return "EOF";
        case UNKNOWN_TOKEN: return "UNKNOWN_TOKEN";
        
        default: return "UNKNOWN_TOKEN";
    }
}

// add here which line
void printSyntaxError(int expected, int found, int line) {
    printf("Syntax error (Line %d): expected token %d: %s but found %d: %s\n",
            line,
           expected, tokenTypeToString(expected),
           found, tokenTypeToString(found));
}

// This can be edited to just return error codes
int match(int expected) {
    if (current_token_parse.type == expected) {
        // printf("Consumed: %s\n", current_token_parse.name);
        parse_index++;
        return 1;
    }

    printSyntaxError(expected, current_token_parse.type, current_token_parse.line);
    panicRecovery();
    return 0;
}

void panicRecovery() {
    // Track brace depth during recovery
    int brace_depth = 0;
    
    while (parse_index < count && current_token_parse.type != EOF_TOKEN) {
        
        // Track braces as we skip
        if (current_token_parse.type == LCBRACE) {
            brace_depth++;
            parse_index++;
            continue;
        }
        
        if (current_token_parse.type == RCBRACE) {
            if (brace_depth > 0) {
                // We're exiting a block we entered during recovery
                brace_depth--;
                parse_index++;
                continue;
            } else {
                // We've hit a closing brace at our original level
                // Don't consume it - let the normal parser handle it
                return;
            }
        }
        
        // Only treat these as synchronization points if we're at base level
        if (brace_depth == 0) {
            // Semicolon ends a statement
            if (current_token_parse.type == SEMICOLON) {
                parse_index++; // Consume the semicolon
                return;
            }
            
            // Statement-starting keywords (NOT elif/else - they're not valid alone)
            if (current_token_parse.type == IF ||
                current_token_parse.type == REPEAT ||
                current_token_parse.type == DISPLAY ||
                current_token_parse.type == STOP ||
                current_token_parse.type == CONTINUE ||
                current_token_parse.type == IDENT) {
                // Found a statement starter - stop here without consuming
                return;
            }
            
            // Type keywords (declaration statements)
            if (current_token_parse.type == NUMBER ||
                current_token_parse.type == DECIMAL ||
                current_token_parse.type == TEXT ||
                current_token_parse.type == BOOL ||
                current_token_parse.type == SYMBOL ||
                current_token_parse.type == LIST) {
                // Found a declaration - stop here without consuming
                return;
            }
        }
        
        // Not at a safe stopping point yet, keep skipping
        parse_index++;
    }
}

// --------- GRAMMAR ---------
ASTNode* parseProgram() {
    ASTNode* program = createNode(NODE_LEXC_PROGRAM, NULL, 0);
    ASTNode* stmtList = parseStatementList();
    addChild(program, stmtList);
    return program;
}

ASTNode* parseStatementList() {
    ASTNode* stmtList = createNode(NODE_STMNT_LIST, NULL, 0);
    
    while (parse_index < count &&
           current_token_parse.type != EOF_TOKEN &&
        current_token_parse.type != RCBRACE)
    {
        ASTNode* stmt = parseStatement();
        if (stmt) {
            addChild(stmtList, stmt);
        }

        if (current_token_parse.type == SEMICOLON) {
            match(SEMICOLON);
        }
    }
    
    return stmtList;
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

ASTNode* parseStatement() {
    switch (current_token_parse.type) {
        case IF:
        case REPEAT:
        case LCBRACE:     
            return parseCompoundStatement();

        default: {
            ASTNode* stmt = parseSimpleStatement();
            if (!match(SEMICOLON)) return stmt;
            return stmt;
        }
    }
}

ASTNode* parseCompoundStatement() {
    if (current_token_parse.type == IF) {
        return parseConditionalStmnt();  // handles if (...) { ... } [elif...] [else...]
    }
    if (current_token_parse.type == REPEAT) {
        return parseIterStmnt();         // handles repeat(...) { ... }
    }
    // if (current_token_parse.type == LCBRACE) {
    //     if (!match(LCBRACE)) return NULL;
    //     parseStatementList();
    //     if (!match(RCBRACE)) return NULL;
    //     return NULL;
    // }

    // unexpected — fallback
    printf("Syntax error (Line %d): unexpected token %s\n", current_token_parse.line, current_token_parse.name);
    panicRecovery();
    return NULL;
}

ASTNode* parseSimpleStatement() {
    switch (current_token_parse.type) {
        case NUMBER: case DECIMAL: case TEXT: case BOOL: case SYMBOL: case LIST:
            return parseDecStmnt();    // declaration (no semicolon here)
        case IDENT:
            return parseAssStmnt();    // assignment or expression start
        case DISPLAY:
            return parseOutputStmnt();
        case STOP: case CONTINUE: {   
            ASTNode* jump = createNode(NODE_JUMP_STMNT, tokenTypeToString(current_token_parse.type), current_token_parse.type);
            parseJumpStmnt(jump);
            return jump;
        }
        default:
            printf("Syntax error (Line %d): unexpected token %s\n", current_token_parse.line, current_token_parse.name);
            panicRecovery();
            return NULL;
    }
}

// ---- Declaration Statement ---------

ASTNode* parseDecStmnt(){
    int dataType = parseDataType();
    ASTNode* decNode = createNode(NODE_DECLARATION_STMNT, tokenTypeToString(dataType), dataType);
    // parseIdList();
    parseDecList(decNode);
    // if (!match(IDENT)) return;

    // parseDecItemSuffix();
    return decNode;
}

void parseDecList(ASTNode* parent){
    parseDecItem(parent);
    parseDecListTail(parent);
}

void parseDecListTail(ASTNode* parent){
    if (current_token_parse.type == COMMA){
        if(!match(COMMA)) return;
        parseDecItem(parent);
        parseDecListTail(parent);
    }
}

ASTNode* parseDecItem(ASTNode* parent){
    if (current_token_parse.type != IDENT) {
        match(IDENT);
        return NULL;
    }
    
    ASTNode* idNode = createNode(NODE_IDENTIFIER, current_token_parse.lexeme, IDENT);
    match(IDENT);
    
    parseDecItemSuffix(idNode);
    addChild(parent, idNode);
    return idNode;
}

void parseDecItemSuffix(ASTNode* parent){
    if (current_token_parse.type == ASSIGN_OP){
        if(!match(ASSIGN_OP)) return;

        // special-case: allow `get(...)` directly in declarations
        if (current_token_parse.type == GET) {
            ASTNode* inputNode = parseInputStmnt();
            addChild(parent, inputNode);
            return;
        }

        // otherwise parse a normal expression
        ASTNode* expr = parseExpr();
        addChild(parent, expr);
    }
}


// --------- INPUT STATEMENT ---------

ASTNode* parseInputStmnt() {
    ASTNode* inputNode = createNode(NODE_INPUT_STMNT, "GET", GET);
    
    if (!match(GET)) return inputNode;
    if (!match(LEFT_PAREN)) return inputNode;

    int dataType = parseDataType();
    ASTNode* typeNode = createNode(NODE_DATA_TYPE, tokenTypeToString(dataType), dataType);
    addChild(inputNode, typeNode);

    if (!match(RIGHT_PAREN)) return inputNode;
    
    return inputNode;
}

// --------- DATATYPE ---------

int parseDataType() {
    switch (current_token_parse.type) {
        case NUMBER:
        case DECIMAL:
        case BOOL:
        case SYMBOL:
        case TEXT:
        case LIST: {
            int type = current_token_parse.type;
            match(current_token_parse.type);
            return type;
        }

        default:
            printf("Syntax error (Line %d): expected a data type, found %s\n",
                   current_token_parse.line, current_token_parse.name);
            return 0;
    }
}

// ----- OUTPUT ------

ASTNode* parseOutputStmnt() {
    ASTNode* outputNode = createNode(NODE_OUTPUT_STMNT, "DISPLAY", DISPLAY);
    
    if (!match(DISPLAY)) return outputNode;
    if (!match(LEFT_PAREN)) return outputNode;

    ASTNode* expr = parseExpr();
    addChild(outputNode, expr);

    if (!match(RIGHT_PAREN)) return outputNode;
    
    return outputNode;
}

// -----  ASSIGNMENT -------

ASTNode* parseAssStmnt() {
    if (current_token_parse.type != IDENT) {
        match(IDENT);
        return NULL;
    }
    
    ASTNode* assignNode = createNode(NODE_ASSMNT_STMNT, NULL, ASSIGN_OP);
    ASTNode* idNode = createNode(NODE_IDENTIFIER, current_token_parse.lexeme, IDENT);
    addChild(assignNode, idNode);
    
    match(IDENT);
    if (!match(ASSIGN_OP)) return assignNode;

    if (current_token_parse.type == GET) {
        ASTNode* inputNode = parseInputStmnt();
        addChild(assignNode, inputNode);
        return assignNode;
    }

    ASTNode* expr = parseExpr();
    addChild(assignNode, expr);
    
    return assignNode;
}

// ----------- CONDITIONAL ------------

ASTNode* parseConditionalStmnt(){
    ASTNode* ifNode = createNode(NODE_CONDITIONAL_STMNT, "if", IF);

    if (!match(IF)) return ifNode;
    if (!match(LEFT_PAREN)) return ifNode;
    
    ASTNode* condition = parseExpr();
    addChild(ifNode, condition);
    
    if (!match(RIGHT_PAREN)) return ifNode;
    if (!match(LCBRACE)) return ifNode;
    
    ASTNode* thenBlock = parseStatementList();
    addChild(ifNode, thenBlock);
    
    if (!match(RCBRACE)) return ifNode;

    parseElifList(ifNode);
    parseElseOpt(ifNode);
    
    return ifNode;
}

void parseElifList(ASTNode* parent){
    // parse zero or more `elif (cond) { ... }`
    while (current_token_parse.type == ELIF) {
        ASTNode* elifNode = createNode(NODE_ELIF_CLAUSE, "elif", ELIF);
        
        if (!match(ELIF)) return;
        if (!match(LEFT_PAREN)) return;
        
        ASTNode* condition = parseExpr();
        addChild(elifNode, condition);
        
        if (!match(RIGHT_PAREN)) return;
        if (!match(LCBRACE)) return;
        
        ASTNode* elifBlock = parseStatementList();
        addChild(elifNode, elifBlock);
        
        if (!match(RCBRACE)) return;
        
        addChild(parent, elifNode);
    }
}

void parseElseOpt(ASTNode* parent){
    if (current_token_parse.type == ELSE){
        ASTNode* elseNode = createNode(NODE_ELSE_CLAUSE, "else", ELSE);
        
        if (!match(ELSE)) return;
        if (!match(LCBRACE)) return;
        
        ASTNode* elseBlock = parseStatementList();
        addChild(elseNode, elseBlock);
        
        if (!match(RCBRACE)) return;
        
        addChild(parent, elseNode);
    }
}


// ------ ITERATIVE --------
ASTNode* parseIterStmnt(){
    ASTNode* repeatNode = createNode(NODE_REPEAT, "repeat", REPEAT);
    
    if (!match(REPEAT)) return repeatNode;
    if (!match(LEFT_PAREN)) return repeatNode;
    
    ASTNode* count = parseExpr();
    addChild(repeatNode, count);
    
    if (!match(RIGHT_PAREN)) return repeatNode;
    if (!match(LCBRACE)) return repeatNode;
    
    ASTNode* body = parseStatementList();
    addChild(repeatNode, body);
    
    if (!match(RCBRACE)) return repeatNode;

    return repeatNode;
}

void parseJumpStmnt(ASTNode* parent){
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

ASTNode* parseExpr () {
    return parseBoolExpr ();
}

ASTNode* parseBoolExpr () {
    ASTNode* left = parseRelExpr (); // Should be AND_TERM but skipping for now

    while (current_token_parse.type == OR_OP)
    {
        int op = current_token_parse.type;
        match(OR_OP);
        ASTNode* right = parseRelExpr(); // Should be AND_TERM
        
        ASTNode* binOp = createNode(NODE_LOGICAL_OP, "or", op);
        addChild(binOp, left);
        addChild(binOp, right);
        left = binOp;
    }
    
    return left;
}

ASTNode* parseRelExpr (){
    ASTNode* left = parseAddExpr();

    switch (current_token_parse.type)
    {
    case EQUALITY_OP:
    case INEQUALITY_OP:
    case GREATER_THAN_OP:
    case LESS_THAN_OP:
    case GREATER_EQUAL_OP:
    case LESS_EQUAL_OP: {
        int op = current_token_parse.type;
        match(current_token_parse.type);
        ASTNode* right = parseAddExpr();
        
        ASTNode* relOp = createNode(NODE_COMPARE_OP, tokenTypeToString(op), op);
        addChild(relOp, left);
        addChild(relOp, right);
        return relOp;
    }
    
    default:
        break;
    }
    
    return left;
}

ASTNode* parseAddExpr (){
    ASTNode* left = parseTerm();
        
    while(current_token_parse.type == ADDITION_OP ||
            current_token_parse.type == SUBTRACT_OP){
        int op = current_token_parse.type;
        match(current_token_parse.type);
        ASTNode* right = parseTerm();
        
        ASTNode* addOp = createNode(NODE_ADDITIVE_OP, tokenTypeToString(op), op);
        addChild(addOp, left);
        addChild(addOp, right);
        left = addOp;
    }
    
    return left;
}

ASTNode* parseTerm(){
    ASTNode* left = parsePower();

    while(current_token_parse.type == MULTIPLY_OP ||
    current_token_parse.type == DIVIDE_OP ||
    current_token_parse.type == FLOORDIV_OP ||
    current_token_parse.type == MODULO_OP) {
        int op = current_token_parse.type;
        match(current_token_parse.type);
        ASTNode* right = parsePower();
        
        ASTNode* multOp = createNode(NODE_MULTIPLICATIVE_OP, tokenTypeToString(op), op);
        addChild(multOp, left);
        addChild(multOp, right);
        left = multOp;
    }
    
    return left;
}

ASTNode* parsePower() {
    ASTNode* left = parseUnary();

    while(current_token_parse.type == EXPONENT_OP){
        int op = current_token_parse.type;
        match(EXPONENT_OP);
        ASTNode* right = parseUnary();
        
        ASTNode* powOp = createNode(NODE_POWER, "^", op);
        addChild(powOp, left);
        addChild(powOp, right);
        left = powOp;
    }
    
    return left;
}

ASTNode* parseUnary() {
    if (current_token_parse.type == ADDITION_OP ||
        current_token_parse.type == SUBTRACT_OP ||
        current_token_parse.type == PRE_INCREMENT_OP ||
        current_token_parse.type == PRE_DECREMENT_OP ||
        current_token_parse.type == NOT_OP){
            
        int op = current_token_parse.type;
        match(current_token_parse.type);
        ASTNode* operand = parseUnary();
        
        ASTNode* unaryOp = createNode(NODE_UNARY_EXPR, tokenTypeToString(op), op);
        addChild(unaryOp, operand);
        return unaryOp;
    }
    
    return parsePostfix();
}

ASTNode* parsePostfix(){
    ASTNode* primary = parsePrimary();

    while(current_token_parse.type == POST_INCREMENT_OP ||
        current_token_parse.type == POST_DECREMENT_OP) {
        int op = current_token_parse.type;
        match(current_token_parse.type);
        
        ASTNode* postOp = createNode(NODE_POSTFIX_EXPR, tokenTypeToString(op), op);
        addChild(postOp, primary);
        primary = postOp;
    }
    
    return primary;
}

ASTNode* parsePrimary(){
    switch(current_token_parse.type) {
        case NUM_LIT:
        case DEC_LIT:
        case SYM_LIT:
        case TRUE:
        case FALSE:
        case TEXT_LIT: {
            ASTNode* lit = createNode(NODE_PRIMARY, current_token_parse.lexeme, current_token_parse.type);
            match(current_token_parse.type);
            return lit;
        }

        case IDENT: {
            ASTNode* id = createNode(NODE_PRIMARY, current_token_parse.lexeme, IDENT);
            match(IDENT);
            return id;
        }

        case LEFT_PAREN: {
            match(LEFT_PAREN);
            ASTNode* expr = parseExpr();
            match(RIGHT_PAREN);
            return expr;
        }

        default:
            printf("Syntax error: expected literal, found %s\n",
                   current_token_parse.name);
            panicRecovery();
            return NULL;
    } 
}