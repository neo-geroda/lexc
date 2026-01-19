# LEX-C
Lightweight Enhanced eXecution Code

## How to Run

1. Open the folder in terminal `../lexc`

2. Compile the file by typing `gcc src/main.c src/parser.c src/lexer.c src/token_stream.c src/token_list.c src/symbol_table.c src/lookupkeywords.c src/parser_tree.c  -I include -g -Wall -o compiler.exe`

3. Run by typing `.\compiler.exe tests/test8.lexc`
