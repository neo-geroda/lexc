#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"  // Include the lookupKeywords function
#include "../include/tokens.h"          // Token definitions
#include "../include/symbol_table.h"    // Symbol table functions
#include "../include/lexer.h"          // Lexer function declarations
#include "../include/token_stream.h"    // Token stream wrapper
#include <stdlib.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
int nextChar;
int lexLen;
char current_token[100];
int nextToken;
FILE *in_fp;
int lastTokenReturned = -1;
int lineNumber = 1;
int tokenLineNumber = 1;  /* Line number where current token starts */

/* Function declarations */
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
int lookupKeywords(char *s);

Token *tokens = NULL;      // dynamic array
size_t count = 0;          // how many items we have
size_t capacity = 0;       // current allocated size

   int runLexerTest(int argc, char **argv) { 
        const char *filename = "test.lexc";
        if (argc > 1) filename = argv[1];

        const char *dot = strrchr(filename, '.');
        if (!dot || strcmp(dot, ".lexc") != 0) {
            printf("ERROR - input file must have a .lexc extension\n");
            return 1;
        }

        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("ERROR - cannot open %s\n", filename);
            return 1;
        }

        printf("------------------------------------------------------------------------------\n");
        printf("|Line\t|Token Key\t|Token Name\t\t|Lexeme\t\n");
        printf("------------------------------------------------------------------------------\n");

        init_lexer_stream(fp);

        Token t;
        do {
            t = next_token();

            // Skip single-line and multi-line comments
            if (t.type != SINGLE_LINE_COMMENT && t.type != MULTILINE_COMMENT) {
                if (count == capacity) {
                    capacity = capacity == 0 ? 4 : capacity * 2;
                    tokens = realloc(tokens, capacity * sizeof(Token));
                    if (!tokens) {
                        fprintf(stderr, "Memory allocation failed\n");
                        exit(1);
                    }
                }

                tokens[count++] = t; 
                // printf("Appended successfully!\n");
            }

            printf("|%-5d\t|%-15d|%-23s|%s\n", t.line, t.type, t.name, t.lexeme);

        } while (t.type != EOF_TOKEN);

        printSymbolTable();

        close_lexer_stream();
        fclose(fp);
        return 0;
    }


    void readSingleLineComment(void) {
        addChar(); 
        getChar();
        while (nextChar != '\n' && nextChar != EOF) {
            addChar();
            getChar();
        }
    }

    void readMultiLineComment(void) {
        while (nextChar != EOF) {
            if (nextChar == '~') {
                addChar();
                getChar();
                return;
            } else if (nextChar == '\n') {
                if (lexLen <= 98) {
                    lexeme[lexLen++] = ' ';
                    lexeme[lexLen] = '\0';
                }
                getChar();
            } else {
                addChar();
                getChar();
            }
        }
    }

    int isValidIdentifier(char *id) {
        if (id[0] == '_') {
            return 0;
        }
        for (int i = 0; id[i] != '\0'; i++) {
            if (id[i] == '_' && id[i+1] == '_') {
                return 0;
            }
        }
        return 1;
    }

    int lookupOperators(char ch) {
        switch (ch) {
            case '(':
                nextToken = LEFT_PAREN;
                break;
            case ')':
                nextToken = RIGHT_PAREN;
                break;
            case '[':
                nextToken = LSQBRACKET;
                break;
            case ']':
                nextToken = RSQBRACKET;
                break;
            case '{':
                nextToken = LCBRACE;
                break;
            case '}':
                nextToken = RCBRACE;
                break;
            case ';':
                nextToken = SEMICOLON;
                break;
            case ',':
                nextToken = COMMA;
                break;
            case ':':
                nextToken = COLON;
                break;
            case '+':
                if (nextChar == '+') {
                    addChar();
                    getChar();
                    if (lastTokenReturned == IDENT || lastTokenReturned == DIGIT) {
                        nextToken = POST_INCREMENT_OP;
                    } else {
                        nextToken = PRE_INCREMENT_OP;
                    }
                } else if (charClass == DIGIT) {
                    addChar();
                    getChar();
                    nextToken = POS_OP;
                } else {
                    nextToken = ADDITION_OP;
                }
                break;
            case '-':
                if (nextChar == '-') {
                    addChar();
                    getChar();
                    if (lastTokenReturned == IDENT || lastTokenReturned == DIGIT) {
                        nextToken = POST_DECREMENT_OP;
                    } else {
                        nextToken = PRE_DECREMENT_OP;
                    }
                } else if (charClass == DIGIT) {
                    addChar();
                    getChar();
                    nextToken = NEG_OP;
                } else {
                    nextToken = SUBTRACT_OP;
                }
                break;
            case '*':
                nextToken = MULTIPLY_OP;
                break;
            case '/':
                if (nextChar == '/') {
                    addChar();
                    getChar();
                    nextToken = FLOORDIV_OP;
                } else {
                nextToken = DIVIDE_OP;
                }
                break;
            case '%':
                nextToken = MODULO_OP;
                break;
            case '^':
                nextToken = EXPONENT_OP;
                break;
            case '=':
                if (nextChar == '=') {
                    addChar();
                    getChar();
                    nextToken = EQUALITY_OP;
                } else {
                    nextToken = ASSIGN_OP;
                }
                break;
            case '!':
                if (nextChar == '=') {
                    addChar();
                    getChar();
                    nextToken = INEQUALITY_OP;
                } else {
                    nextToken = NOT_OP;
                }
                break;
            case '<':
                if (nextChar == '=') {
                    addChar();
                    getChar(); 
                    nextToken = LESS_EQUAL_OP;
                } else {
                    nextToken = LESS_THAN_OP;
                }
                break;
            case '>':
                if (nextChar == '=') {
                    addChar();
                    getChar();
                    nextToken = GREATER_EQUAL_OP;
                } else {
                    nextToken = GREATER_THAN_OP;
                }
                break;
            default:
                nextToken = UNKNOWN_TOKEN;
                break;
        }
        return nextToken;
    }

    void addChar(void) {
        if (lexLen <= 98) {
            lexeme[lexLen++] = (char)nextChar;
            lexeme[lexLen] = '\0';
        } else {
            printf("ERROR - lexeme is too long\n");
            lexLen = 0;
            lexeme[0] = '\0';
        }
    }

    void getChar(void) {
        if (in_fp == NULL) {
            nextChar = EOF;
            charClass = EOF;
            return;
        }

        int c = getc(in_fp);
        if (c == EOF) {
            nextChar = EOF;
            charClass = EOF;
            return;
        }

        nextChar = c;
        if (c == '\n') {
            lineNumber++;
        }
        if (isalpha((unsigned char)c)) {
            charClass = LETTER;
        } else if (isdigit((unsigned char)c)) {
            charClass = DIGIT;
        } else {
            charClass = UNKNOWN;
        }
    }

    void getNonBlank(void) {
        while (nextChar != EOF && isspace((unsigned char)nextChar)) {
            getChar();
        }
    }


    int lex() {
        lexLen = 0;

        /* Check if already at EOF */
        if (charClass == EOF) {
            nextToken = EOF_TOKEN;
            return nextToken;
        }

        getNonBlank();

        /* Check EOF after skipping whitespace */
        if (charClass == EOF) {
            nextToken = EOF_TOKEN;
            return nextToken;
        }

        /* Capture line number at start of token (after whitespace skipped) */
        tokenLineNumber = lineNumber;

        switch (charClass) {
            case LETTER:
                addChar();
                getChar();
                while (charClass == LETTER || charClass == DIGIT || (nextChar == '_' && lexeme[lexLen-1] != '_')) {
                    if (nextChar == '_') {
                        addChar();
                        getChar();
                    } else {
                        addChar();
                        getChar();
                    }
                }
                /* Check for invalid identifier format and classify accordingly */
                if (!isValidIdentifier(lexeme)) {
                    nextToken = UNKNOWN_TOKEN;
                } else {
                    nextToken = lookupKeywords(lexeme);
                    if(nextToken == IDENT) {
                        addSymbol(lexeme, CAT_VAR, TYPE_NONE, 0, tokenLineNumber);
                    }
                }
                break;
            case DIGIT:
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                } if (nextChar == '.') {
                    addChar();
                    getChar();
                    while (charClass == DIGIT) {
                        addChar();
                        getChar();
                    }
                    nextToken = DEC_LIT;
                } else {
                    nextToken = NUM_LIT;
                }
                break;
            case UNKNOWN:
                char start_char = nextChar;
                addChar();
                getChar();

                if(start_char == '~') {
                    if (nextChar == '~') {
                        readSingleLineComment();
                        nextToken = SINGLE_LINE_COMMENT;
                    } else {
                        readMultiLineComment();
                        nextToken = MULTILINE_COMMENT;
                    }
                } else if (start_char == '"') {
                    while (nextChar != EOF && nextChar != '\"') {
                        addChar();
                        getChar();
                    }
                    if (nextChar == '\"') {
                        addChar();
                        getChar();
                    } else {
                        printf("ERROR - unclosed string literal\n");
                    }
                    nextToken = TEX_LIT;
                } else if (start_char == '\'') {
                    while (nextChar != EOF && nextChar != '\'') {
                        addChar();
                        getChar();
                    } if (nextChar == '\'') {
                        addChar();
                        getChar();
                    } else {
                        printf("ERROR - unclosed symbol literal\n");
                    }
                    nextToken = SYM_LIT;
                } else if (charClass == LETTER || charClass == DIGIT || nextChar == '_') {
                    while (charClass == LETTER || charClass == DIGIT || (nextChar == '_' && lexeme[lexLen-1] != '_')) {
                        if (nextChar == '_') {
                            addChar();
                            getChar();
                        } else {
                            addChar();
                            getChar();
                        }
                    }
                    nextToken = UNKNOWN_TOKEN;
                } else {
                    lexLen = 1;
                    lexeme[1] = '\0';
                    nextToken = lookupOperators(start_char);
                }
                break;
        }

        lastTokenReturned = nextToken;
        return nextToken;
    }

void lexer_set_input(FILE *in) {
    in_fp = in;
    getChar();
}