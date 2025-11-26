/* Implementation of symbol table functions */
#include <stdio.h>
#include <string.h>
#include "../include/symbol_table.h"

static Symbol symbolTable[MAX_SYMBOLS];
static int symbolCount = 0;

static const char* getCategoryName(SymbolCategory category) {
    switch (category) {
        case CAT_VAR: return "VARIABLE";
        case CAT_FUNC: return "FUNCTION";
        case CAT_PARAM: return "PARAMETER";
        case CAT_CONST: return "CONSTANT";
        default: return "UNDEFINED";
    }
}

static const char* getTypeName(DataType type) {
    switch (type) {
        case TYPE_NUM: return "NUMBER";
        case TYPE_DEC: return "DECIMAL";
        case TYPE_SYM: return "SYMBOL";
        case TYPE_TEX: return "TEXT";
        case TYPE_BOOL: return "BOOLEAN";
        case TYPE_VOID: return "VOID";
        default: return "NONE";
    }
}

void addSymbol(char *lexeme, SymbolCategory cat, DataType type, int scope, int line) {
    if (symbolCount >= MAX_SYMBOLS) return;
    /* If symbol already exists in same scope, keep it and do not add duplicate. */
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, lexeme) == 0 && symbolTable[i].scopeLevel == scope) {
            /* Do not add duplicate; keep original lineDeclared */
            return;
        }
    }
    strncpy(symbolTable[symbolCount].name, lexeme, MAX_LEN - 1);
    symbolTable[symbolCount].name[MAX_LEN - 1] = '\0';
    symbolTable[symbolCount].category = cat;
    symbolTable[symbolCount].type = type;
    symbolTable[symbolCount].scopeLevel = scope;
    symbolTable[symbolCount].lineDeclared = line;
    symbolCount++;
}

void printSymbolTable() {
    printf("\n\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("| SYMBOL TABLE                                                                          |\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-30s | %-15s | %-15s | %-5s | %-5s |\n", "Index", "Name", "Category", "Type", "Scope", "Line");
    printf("-----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("| %-5d | %-30s | %-15s | %-15s | %-5d | %-5d |\n",
               i,
               symbolTable[i].name,
               getCategoryName(symbolTable[i].category),
               getTypeName(symbolTable[i].type),
               symbolTable[i].scopeLevel,
               symbolTable[i].lineDeclared);
    }
    printf("-----------------------------------------------------------------------------------------\n");
}