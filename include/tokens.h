#ifndef TOKENS_H
#define TOKENS_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 9

/* Basic token codes */
#define IDENT 11
#define ASSIGN_OP 20
#define POS_OP 21
#define NEG_OP 22
#define LEFT_PAREN 23
#define RIGHT_PAREN 24

#define UNKNOWN_TOKEN 30

/* Arithmetic Operators */
#define ADDITION_OP 31
#define SUBTRACT_OP 32
#define MULTIPLY_OP 33
#define DIVIDE_OP 34
#define MODULO_OP 35
#define EXPONENT_OP 36
#define FLOORDIV_OP 37
#define PRE_INCREMENT_OP 38
#define POST_INCREMENT_OP 39
#define PRE_DECREMENT_OP 40
#define POST_DECREMENT_OP 41

/* Relational Operators */
#define EQUALITY_OP 42
#define INEQUALITY_OP 43
#define GREATER_THAN_OP 44
#define LESS_THAN_OP 45
#define GREATER_EQUAL_OP 46
#define LESS_EQUAL_OP 47

/* Logical Operators */
#define AND_OP 50
#define OR_OP 51
#define NOT_OP 52

/* Keyword token codes */
#define IF 100
#define ELIF 101
#define ELSE 102
#define REPEAT 103
#define REPEATFOR 104
#define STOP 105
#define CONTINUE 106
#define DISPLAY 107
#define GET 108
#define GIVE 109
#define FUNCTION 110
#define STATIC 111
#define INCLUDE 112
#define TRY 113
#define BUNDLE 114
#define VOID 115
#define IN 116
#define ONFAIL 117
// this has no end

/* Data Types */
#define NUMBER 120
#define DECIMAL 121
#define SYMBOL 122
#define BOOL 123
#define STRING 124 // This is a text already
#define LIST 125
#define TEXT 126

/* Literals */
#define TRUE 130
#define FALSE 131
#define NOTHING 132
#define NUM_LIT 133
#define DEC_LIT 134
#define TEXT_LIT 135
#define SYM_LIT 136

/* Delimiters */
#define SEMICOLON 140
#define COMMA 141
#define LSQBRACKET 142
#define RSQBRACKET 143
#define LCBRACE 144
#define RCBRACE 145
#define DQUOTE 146
#define SQUOTE 147
#define COLON 148

/* Noise Words */
#define NOISE_WORD 150

/* Comments */
#define SINGLE_LINE_COMMENT 151
#define MULTILINE_COMMENT 152

/* End of File */
#define EOF_TOKEN 200

#include "token_list.h"

#define token(key) token_name(key)

#endif /* TOKENS_H */