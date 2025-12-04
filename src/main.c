#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "../include/lookupkeywords.h"  // Include the lookupKeywords function
#include "../include/tokens.h"          // Token definitions
#include "../include/symbol_table.h"    // Symbol table functions
#include "../include/lexer.h"           // Lexer function declarations

int main(int argc, char **argv) {
    // Run the lexer first
    int res = runLexerTest(argc, argv);
    if (res != 0) {
        return res;  // exit if there was an error
    }

    // // Optionally, print all tokens
    // printf("\n--- Tokens ---\n");
    // for (size_t i = 0; i < count; i++) {
    //     Token t = tokens[i];
    //     printf("Token %zu: Line=%d, Type=%d, Name=%s, Lexeme=%s\n",
    //            i, t.line, t.type, t.name, t.lexeme);
    // }

    // Run the parser on the tokens array
    printf("\n--- Parsing ---\n");
    parse();

    return 0;
}
