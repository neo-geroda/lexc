#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

#define MAX_SYMBOLS 1000
#define MAX_LEN 100

typedef enum {
    CAT_VAR,
    CAT_FUNC,
    CAT_PARAM,
    CAT_CONST,
    CAT_UNDEFINED
} SymbolCategory;

typedef enum {
    TYPE_NUM,
    TYPE_DEC,
    TYPE_SYM,
    TYPE_TEX,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_NONE
} DataType;

/* Symbol record */
typedef struct {
    char name[MAX_LEN];
    SymbolCategory category;
    DataType type;
    int scopeLevel;
    int lineDeclared;
} Symbol;

/* symbol table API */
void addSymbol(char *lexeme, SymbolCategory cat, DataType type, int scope, int line);
void printSymbolTable(void);

#endif /* SYMBOL_TABLE_H */
