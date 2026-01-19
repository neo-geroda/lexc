#ifndef PARSERTREE_H
#define PARSERTREE_H

#include <stdio.h>
#include "tokens.h"          // for token types
#include "lexer.h"           // for next_token()
#include "token_stream.h"    // if needed for the stream
#include "symbol_table.h"    // if parser interacts with symbol table

void tree_parse();

#endif // PARSERTREE_H