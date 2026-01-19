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
    ASTNode* node = (ASTNode*)calloc(1, sizeof(ASTNode));
    if (!node) return NULL;
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->token_type = token_type;
    return node;
}

void addChild(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity = parent->child_capacity == 0 ? 4 : parent->child_capacity * 2;
        ASTNode** new_children = (ASTNode**)realloc(parent->children, 
                                              parent->child_capacity * sizeof(ASTNode*));
        if (!new_children) exit(1);
        parent->children = new_children;
    }
    parent->children[parent->child_count++] = child;
}

void freeAST(ASTNode* node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        freeAST(node->children[i]);
    }
    free(node->children);
    if (node->value) free(node->value);
    free(node);
}

const char* nodeTypeToString(NodeType type) {
    switch(type) {
        case NODE_LEXC_PROGRAM: return "<LexCProgram>";
        case NODE_STMNT_LIST: return "<STMNT_LIST>";
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
        case NODE_EXPR: return "<EXPR>";
        case NODE_BOOL_EXPR: return "<BOOL_EXPR>";
        case NODE_REL_EXPR: return "<REL_EXPR>";
        case NODE_ADDITIVE_EXPR: return "<ADDITIVE_EXPR>";
        case NODE_TERM: return "<TERM>";
        case NODE_POWER: return "<POWER>";
        case NODE_UNARY_EXPR: return "<UNARY_EXPR>";
        case NODE_POSTFIX_EXPR: return "<POSTFIX_EXPR>";
        case NODE_PRIMARY: return "<PRIMARY>";
        case NODE_DATA_TYPE: return "<DATA_TYPE>";
        case NODE_IDENTIFIER: return "id";
        case NODE_ADDITIVE_OP: return "<ADDITIVE_OP>";
        case NODE_MULTIPLICATIVE_OP: return "<MULTIPLICATIVE_OP>";
        case NODE_COMPARE_OP: return "<COMPARE_OP>";
        case NODE_LOGICAL_OP: return "<LOGICAL_OP>";
        case NODE_ELIF_CLAUSE: return "elif";
        case NODE_ELSE_CLAUSE: return "else";
        default: return "<UNKNOWN>";
    }
}

void printAST(ASTNode* node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s", nodeTypeToString(node->type));
    if (node->value) printf("(%s)", node->value);
    printf("\n");
    for (int i = 0; i < node->child_count; i++) {
        printAST(node->children[i], indent + 1);
    }
}

// Function Declarations
ASTNode* parseProgram();
ASTNode* parseStatementList();
ASTNode* parseStatement();
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
void parseJumpStmnt(ASTNode* parent);
ASTNode* parseExpr();
void parseElifList(ASTNode* parent);
void parseElseOpt(ASTNode* parent);
void panicRecovery();
ASTNode* parseBoolExpr();
ASTNode* parseRelExpr();
ASTNode* parseAddExpr();
ASTNode* parseTerm();
ASTNode* parsePower();
ASTNode* parseUnary();
ASTNode* parsePostfix();
ASTNode* parsePrimary();
int match(int expected);

// Globals
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

const char* tokenTypeToString(int t) {
    switch(t) {
        case IDENT: return "IDENTIFIER";
        case ASSIGN_OP: return "=";
        case LEFT_PAREN: return "(";
        case RIGHT_PAREN: return ")";
        case ADDITION_OP: return "+";
        case SUBTRACT_OP: return "-";
        case MULTIPLY_OP: return "*";
        case DIVIDE_OP: return "/";
        case MODULO_OP: return "%";
        case EXPONENT_OP: return "**";
        case EQUALITY_OP: return "==";
        case IF: return "IF";
        case ELIF: return "ELIF";
        case ELSE: return "ELSE";
        case REPEAT: return "REPEAT";
        case DISPLAY: return "DISPLAY";
        case GET: return "GET";
        case LCBRACE: return "{";
        case RCBRACE: return "}";
        case SEMICOLON: return ";";
        case NUMBER: return "NUMBER";
        case DECIMAL: return "DECIMAL";
        case TEXT: return "TEXT";
        case BOOL: return "BOOL";
        case NUM_LIT: return "NUMBER_LITERAL";
        case DEC_LIT: return "DECIMAL_LITERAL";
        case TEXT_LIT: return "TEXT_LITERAL";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case EOF_TOKEN: return "EOF";
        default: return "TOKEN";
    }
}

void printSyntaxError(int expected, int found, int line) {
    printf("Syntax error (Line %d): expected %s but found %s\n",
            line, tokenTypeToString(expected), tokenTypeToString(found));
}

int match(int expected) {
    if (current_token_parse.type == expected) {
        parse_index++;
        return 1;
    }
    printSyntaxError(expected, current_token_parse.type, current_token_parse.line);
    panicRecovery();
    return 0;
}

void panicRecovery() {
    int brace_depth = 0;
    if (current_token_parse.type == EOF_TOKEN) return;
    while (parse_index < count && current_token_parse.type != EOF_TOKEN) {
        if (current_token_parse.type == LCBRACE) { brace_depth++; parse_index++; continue; }
        if (current_token_parse.type == RCBRACE) {
            if (brace_depth > 0) { brace_depth--; parse_index++; continue; }
            else return; 
        }
        if (brace_depth == 0) {
            if (current_token_parse.type == SEMICOLON) return;
            switch(current_token_parse.type) {
                case IF: case REPEAT: case DISPLAY: case STOP: case CONTINUE:
                case NUMBER: case DECIMAL: case TEXT: case BOOL: return;
            }
        }
        parse_index++;
    }
}

// --------- GRAMMAR ---------

ASTNode* parseProgram() {
    ASTNode* program = createNode(NODE_LEXC_PROGRAM, NULL, 0);
    addChild(program, parseStatementList());
    return program;
}

ASTNode* parseStatementList() {
    ASTNode* stmtList = createNode(NODE_STMNT_LIST, NULL, 0);
    while (parse_index < count && current_token_parse.type != EOF_TOKEN && current_token_parse.type != RCBRACE) {
        size_t start = parse_index;
        ASTNode* stmt = parseStatement();
        if (stmt) addChild(stmtList, stmt);
        
        if (current_token_parse.type == SEMICOLON) match(SEMICOLON);
        else if (parse_index == start) parse_index++;
    }
    return stmtList;
}

ASTNode* parseStatement() {
    switch (current_token_parse.type) {
        case IF: case REPEAT: case LCBRACE: return parseCompoundStatement();
        default: {
            ASTNode* stmt = parseSimpleStatement();
            if (stmt) match(SEMICOLON);
            return stmt;
        }
    }
}

ASTNode* parseCompoundStatement() {
    if (current_token_parse.type == IF) return parseConditionalStmnt();
    if (current_token_parse.type == REPEAT) return parseIterStmnt();
    if (current_token_parse.type == LCBRACE) {
        match(LCBRACE);
        ASTNode* list = parseStatementList();
        match(RCBRACE);
        return list;
    }
    return NULL;
}

ASTNode* parseSimpleStatement() {
    switch (current_token_parse.type) {
        case NUMBER: case DECIMAL: case TEXT: case BOOL: case SYMBOL: case LIST: return parseDecStmnt();
        case IDENT: return parseAssStmnt();
        case DISPLAY: return parseOutputStmnt();
        case STOP: case CONTINUE: {
            ASTNode* jmp = createNode(NODE_JUMP_STMNT, tokenTypeToString(current_token_parse.type), current_token_parse.type);
            parseJumpStmnt(jmp);
            return jmp;
        }
        default: return NULL;
    }
}

ASTNode* parseDecStmnt() {
    int type = parseDataType();
    ASTNode* node = createNode(NODE_DECLARATION_STMNT, tokenTypeToString(type), type);
    parseDecList(node);
    return node;
}

void parseDecList(ASTNode* parent) {
    if (parseDecItem(parent)) parseDecListTail(parent);
}

void parseDecListTail(ASTNode* parent) {
    if (current_token_parse.type == COMMA) {
        match(COMMA);
        if (parseDecItem(parent)) parseDecListTail(parent);
    }
}

ASTNode* parseDecItem(ASTNode* parent) {
    if (current_token_parse.type != IDENT) { match(IDENT); return NULL; }
    ASTNode* id = createNode(NODE_IDENTIFIER, current_token_parse.lexeme, IDENT);
    match(IDENT);
    parseDecItemSuffix(id);
    addChild(parent, id);
    return id;
}

void parseDecItemSuffix(ASTNode* parent) {
    if (current_token_parse.type == ASSIGN_OP) {
        match(ASSIGN_OP);
        if (current_token_parse.type == GET) addChild(parent, parseInputStmnt());
        else {
            ASTNode* expr = parseExpr();
            if (expr) addChild(parent, expr);
        }
    }
}

ASTNode* parseInputStmnt() {
    ASTNode* node = createNode(NODE_INPUT_STMNT, "GET", GET);
    match(GET); match(LEFT_PAREN);
    int type = parseDataType();
    addChild(node, createNode(NODE_DATA_TYPE, tokenTypeToString(type), type));
    match(RIGHT_PAREN);
    return node;
}

int parseDataType() {
    int type = current_token_parse.type;
    match(type);
    return type;
}

ASTNode* parseOutputStmnt() {
    ASTNode* node = createNode(NODE_OUTPUT_STMNT, "DISPLAY", DISPLAY);
    match(DISPLAY); match(LEFT_PAREN);
    ASTNode* expr = parseExpr();
    if (expr) addChild(node, expr);
    match(RIGHT_PAREN);
    return node;
}

ASTNode* parseAssStmnt() {
    ASTNode* node = createNode(NODE_ASSMNT_STMNT, NULL, ASSIGN_OP);
    ASTNode* id = createNode(NODE_IDENTIFIER, current_token_parse.lexeme, IDENT);
    addChild(node, id);
    match(IDENT); match(ASSIGN_OP);
    if (current_token_parse.type == GET) addChild(node, parseInputStmnt());
    else {
        ASTNode* expr = parseExpr();
        if (expr) addChild(node, expr);
    }
    return node;
}

ASTNode* parseConditionalStmnt() {
    if (!match(IF)) return NULL;
    if (!match(LEFT_PAREN)) return NULL;
    
    // Check for empty/invalid condition to prevent declaration swallowing
    if (current_token_parse.type == RIGHT_PAREN) {
        printSyntaxError(IDENT, current_token_parse.type, current_token_parse.line);
        match(RIGHT_PAREN);
        if (current_token_parse.type == LCBRACE) {
            match(LCBRACE); 
            freeAST(parseStatementList()); 
            match(RCBRACE);
        }
        return NULL;
    }

    ASTNode* ifNode = createNode(NODE_CONDITIONAL_STMNT, "if", IF);
    ASTNode* cond = parseExpr();
    if (cond) {
        addChild(ifNode, cond);
    } else {
        freeAST(ifNode);
        return NULL;
    }

    if (!match(RIGHT_PAREN)) { freeAST(ifNode); return NULL; }
    if (!match(LCBRACE)) { freeAST(ifNode); return NULL; }
    
    addChild(ifNode, parseStatementList());
    match(RCBRACE);
    
    parseElifList(ifNode);
    parseElseOpt(ifNode);
    return ifNode;
}

void parseElifList(ASTNode* parent) {
    while (current_token_parse.type == ELIF) {
        ASTNode* node = createNode(NODE_ELIF_CLAUSE, "elif", ELIF);
        match(ELIF); match(LEFT_PAREN);
        ASTNode* cond = parseExpr();
        if (cond) addChild(node, cond);
        match(RIGHT_PAREN); match(LCBRACE);
        addChild(node, parseStatementList());
        match(RCBRACE);
        addChild(parent, node);
    }
}

void parseElseOpt(ASTNode* parent) {
    if (current_token_parse.type == ELSE) {
        ASTNode* node = createNode(NODE_ELSE_CLAUSE, "else", ELSE);
        match(ELSE); match(LCBRACE);
        addChild(node, parseStatementList());
        match(RCBRACE);
        addChild(parent, node);
    }
}

ASTNode* parseIterStmnt() {
    if (!match(REPEAT)) return NULL;
    if (!match(LEFT_PAREN)) return NULL;
    
    if (current_token_parse.type == RIGHT_PAREN) {
        printSyntaxError(NUM_LIT, current_token_parse.type, current_token_parse.line);
        match(RIGHT_PAREN);
        if (current_token_parse.type == LCBRACE) {
            match(LCBRACE); freeAST(parseStatementList()); match(RCBRACE);
        }
        return NULL;
    }

    ASTNode* node = createNode(NODE_REPEAT, "repeat", REPEAT);
    ASTNode* loopCount = parseExpr();
    if (loopCount) addChild(node, loopCount);
    
    if (!match(RIGHT_PAREN)) { freeAST(node); return NULL; }
    if (!match(LCBRACE)) { freeAST(node); return NULL; }
    
    addChild(node, parseStatementList());
    match(RCBRACE);
    return node;
}

void parseJumpStmnt(ASTNode* parent) {
    if (current_token_parse.type == CONTINUE) match(CONTINUE);
    else if (current_token_parse.type == STOP) match(STOP);
}

// --------- EXPRESSIONS ---------

ASTNode* parseExpr() { return parseBoolExpr(); }

ASTNode* parseBoolExpr() {
    ASTNode* left = parseRelExpr();
    while (current_token_parse.type == OR_OP || current_token_parse.type == AND_OP) {
        int op = current_token_parse.type;
        match(op);
        ASTNode* bin = createNode(NODE_LOGICAL_OP, tokenTypeToString(op), op);
        addChild(bin, left);
        addChild(bin, parseRelExpr());
        left = bin;
    }
    return left;
}

ASTNode* parseRelExpr() {
    ASTNode* left = parseAddExpr();
    int t = current_token_parse.type;
    if (t == EQUALITY_OP || t == INEQUALITY_OP || t == GREATER_THAN_OP || 
        t == LESS_THAN_OP || t == GREATER_EQUAL_OP || t == LESS_EQUAL_OP) {
        match(t);
        ASTNode* node = createNode(NODE_COMPARE_OP, tokenTypeToString(t), t);
        addChild(node, left);
        addChild(node, parseAddExpr());
        return node;
    }
    return left;
}

ASTNode* parseAddExpr() {
    ASTNode* left = parseTerm();
    while (current_token_parse.type == ADDITION_OP || current_token_parse.type == SUBTRACT_OP) {
        int op = current_token_parse.type;
        match(op);
        ASTNode* node = createNode(NODE_ADDITIVE_OP, tokenTypeToString(op), op);
        addChild(node, left);
        addChild(node, parseTerm());
        left = node;
    }
    return left;
}

ASTNode* parseTerm() {
    ASTNode* left = parsePower();
    while (current_token_parse.type == MULTIPLY_OP || current_token_parse.type == DIVIDE_OP || 
           current_token_parse.type == MODULO_OP) {
        int op = current_token_parse.type;
        match(op);
        ASTNode* node = createNode(NODE_MULTIPLICATIVE_OP, tokenTypeToString(op), op);
        addChild(node, left);
        addChild(node, parsePower());
        left = node;
    }
    return left;
}

ASTNode* parsePower() {
    ASTNode* left = parseUnary();
    while (current_token_parse.type == EXPONENT_OP) {
        match(EXPONENT_OP);
        ASTNode* node = createNode(NODE_POWER, "^", EXPONENT_OP);
        addChild(node, left);
        addChild(node, parseUnary());
        left = node;
    }
    return left;
}

ASTNode* parseUnary() {
    int t = current_token_parse.type;
    if (t == SUBTRACT_OP || t == NOT_OP || t == PRE_INCREMENT_OP || t == PRE_DECREMENT_OP) {
        match(t);
        ASTNode* node = createNode(NODE_UNARY_EXPR, tokenTypeToString(t), t);
        addChild(node, parseUnary());
        return node;
    }
    return parsePostfix();
}

ASTNode* parsePostfix() {
    ASTNode* node = parsePrimary();
    int t = current_token_parse.type;
    if (t == POST_INCREMENT_OP || t == POST_DECREMENT_OP) {
        match(t);
        ASTNode* post = createNode(NODE_POSTFIX_EXPR, tokenTypeToString(t), t);
        addChild(post, node);
        return post;
    }
    return node;
}

ASTNode* parsePrimary() {
    int t = current_token_parse.type;
    if (t == NUM_LIT || t == DEC_LIT || t == TEXT_LIT || t == TRUE || t == FALSE) {
        ASTNode* lit = createNode(NODE_PRIMARY, current_token_parse.lexeme, t);
        match(t); return lit;
    }
    if (t == IDENT) {
        ASTNode* id = createNode(NODE_PRIMARY, current_token_parse.lexeme, IDENT);
        match(IDENT); return id;
    }
    if (t == LEFT_PAREN) {
        match(LEFT_PAREN);
        ASTNode* expr = parseExpr();
        match(RIGHT_PAREN); return expr;
    }
    return NULL;
}