#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/lookupkeywords.h"  // Include the lookupKeywords function
#include "../include/tokens.h"          // Token definitions
#include "../include/symbol_table.h"    // Symbol table functions
#include "../include/lexer.h"          // Lexer function declarations

int main(int argc, char **argv) {
    return runLexerTest(argc, argv);
}