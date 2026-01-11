#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "tokens.h"          // for token types
#include "lexer.h"           // for next_token()
#include "token_stream.h"    // if needed for the stream
#include "symbol_table.h"    // if parser interacts with symbol table

// --------- AST Node Types ---------
// Based on formal EBNF grammar from research paper
typedef enum {
    // Program Structure
    NODE_LEXC_PROGRAM,        // <LexCProgram>
    NODE_STMNT_LIST,          // <STMNT_LIST>
    
    // Statements
    NODE_SIMPLE_STMNT,        // <SIMPLE_STMNT>
    NODE_COMPOUND_STMNT,      // <COMPOUND_STMNT>
    NODE_DECLARATION_STMNT,   // <DECLARATION_STMNT>
    NODE_ASSMNT_STMNT,        // <ASSMNT_STMNT>
    NODE_INPUT_STMNT,         // <INPUT_STMNT>
    NODE_OUTPUT_STMNT,        // <OUTPUT_STMNT>
    NODE_JUMP_STMNT,          // <JUMP_STMNT>
    NODE_CONDITIONAL_STMNT,   // <CONDITIONAL_STMNT>
    NODE_ITERATIVE_STMNT,     // <ITERATIVE_STMNT>
    NODE_REPEAT,              // <REPEAT>
    NODE_REPEAT_FOR,          // <REPEAT_FOR>
    
    // Expression Hierarchy
    NODE_EXPR,                // <EXPR>
    NODE_BOOL_EXPR,           // <BOOL_EXPR>
    NODE_AND_TERM,            // <AND_TERM>
    NODE_NOT_TERM,            // <NOT_TERM>
    NODE_REL_EXPR,            // <REL_EXPR>
    NODE_ADDITIVE_EXPR,       // <ADDITIVE_EXPR>
    NODE_TERM,                // <TERM>
    NODE_POWER,               // <POWER>
    NODE_UNARY_EXPR,          // <UNARY_EXPR>
    NODE_POSTFIX_EXPR,        // <POSTFIX_EXPR>
    NODE_PRIMARY,             // <PRIMARY>
    
    // Components
    NODE_DATA_TYPE,           // <DATA_TYPE>
    NODE_DECL_LIST,           // <DECL_LIST>
    NODE_DECL_ITEM,           // <DECL_ITEM>
    NODE_IDENTIFIER,          // id
    NODE_LITERAL,             // literals
    
    // Operators
    NODE_ADDITIVE_OP,         // <ADDITIVE_OP>
    NODE_MULTIPLICATIVE_OP,   // <MULTIPLICATIVE_OP>
    NODE_COMPARE_OP,          // <COMPARE_OP>
    NODE_LOGICAL_OP,          // logical operators (and, or, not)
    
    // Control Flow Components
    NODE_ELIF_CLAUSE,         // elif clause
    NODE_ELSE_CLAUSE          // else clause
} NodeType;

// --------- AST Node Structure ---------
typedef struct ASTNode {
    NodeType type;
    char* value;               // for identifiers, literals, operators
    int token_type;            // original token type (for operators, literals)
    struct ASTNode** children; // dynamic array of child nodes
    int child_count;
    int child_capacity;
} ASTNode;

// --------- AST Functions ---------
ASTNode* createNode(NodeType type, const char* value, int token_type);
void addChild(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* node);
void printAST(ASTNode* node, int indent);

extern Token current_token_parse;

// Expose the main parse() function - now returns AST
ASTNode* parse();

// Expose your grammar functions if needed externally - now return ASTNodes
ASTNode* parseProgram();
ASTNode* parseStatementList();
ASTNode* parseStatement();
ASTNode* parseInputStmnt();
int parseDataType();
ASTNode* parseOutputStmnt();
ASTNode* parseExpr();
ASTNode* parseAssStmnt();
ASTNode* parseDecStmnt();

// Optionally expose match() if other modules need it
int match(int expected);

// Optionally expose current_token_parse, if other modules access it
extern Token current_token_parse;

#endif // PARSER_H
