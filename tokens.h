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

/* Data Types */
#define NUMBER 120
#define DECIMAL 121
#define SYMBOL 122
#define BOOL 123
#define STRING 124
#define LIST 125
#define TEXT 126

/* Literals */
#define TRUE 130
#define FALSE 131
#define NOTHING 132
#define NUM_LIT 133
#define DEC_LIT 134
#define TEX_LIT 135
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

char *token(int key) {
    switch (key) {
        case IDENT: return "IDENT";
        case ASSIGN_OP: return "ASSIGN_OP";
        case POS_OP: return "POS_OP";
        case NEG_OP: return "NEG_OP";
        case LEFT_PAREN: return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";

        case UNKNOWN_TOKEN: return "UNKNOWN_TOKEN";

        case ADDITION_OP: return "ADDITION_OP";
        case SUBTRACT_OP: return "SUBTRACT_OP";
        case MULTIPLY_OP: return "MULTIPLY_OP";
        case DIVIDE_OP: return "DIVIDE_OP";
        case MODULO_OP: return "MODULO_OP";
        case EXPONENT_OP: return "EXPONENT_OP";
        case FLOORDIV_OP: return "FLOORDIV_OP";
        case PRE_INCREMENT_OP: return "PRE_INCREMENT_OP";
        case POST_INCREMENT_OP: return "POST_INCREMENT_OP";
        case PRE_DECREMENT_OP: return "PRE_DECREMENT_OP";
        case POST_DECREMENT_OP: return "POST_DECREMENT_OP";

        case EQUALITY_OP: return "EQUALITY_OP";
        case INEQUALITY_OP: return "INEQUALITY_OP";
        case GREATER_THAN_OP: return "GREATER_THAN_OP";
        case LESS_THAN_OP: return "LESS_THAN_OP";
        case GREATER_EQUAL_OP: return "GREATER_EQUAL_OP";
        case LESS_EQUAL_OP: return "LESS_EQUAL_OP";

        case AND_OP: return "AND_OP";
        case OR_OP: return "OR_OP";
        case NOT_OP: return "NOT_OP";

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

        case NUMBER: return "NUMBER";
        case DECIMAL: return "DECIMAL";
        case SYMBOL: return "SYMBOL";
        case BOOL: return "BOOL";
        case STRING: return "STRING";
        case LIST: return "LIST";
        case TEXT: return "TEXT";

        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case NOTHING: return "NOTHING";
        case NUM_LIT: return "NUM_LIT";
        case DEC_LIT: return "DEC_LIT";
        case TEX_LIT: return "TEX_LIT";
        case SYM_LIT: return "SYM_LIT";

        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case LSQBRACKET: return "LSQBRACKET";
        case RSQBRACKET: return "RSQBRACKET";
        case LCBRACE: return "LCBRACE";
        case RCBRACE: return "RCBRACE";
        case DQUOTE: return "DQUOTE";
        case SQUOTE: return "SQUOTE";
        case COLON: return "COLON";

        case NOISE_WORD: return "NOISE_WORD";

        case SINGLE_LINE_COMMENT: return "SINGLE_LINE_COMMENT";
        case MULTILINE_COMMENT: return "MULTILINE_COMMENT";

        case EOF_TOKEN: return "EOF_TOKEN";
        
        default: return "UNKNOWN";
    }
}
#endif /* TOKENS_H */