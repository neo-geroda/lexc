# Task 2 Thought Process

## Goal
Implement the clarification in [task.md](../task.md) about why parsing and PDAs are necessary, and connect it to the current LexC implementation without refactoring co-developer code.

## Reasoning Steps
- Read the prompt in [task.md](../task.md) and the existing parser/lexer sources to understand current behavior.
- Recognized the project already uses a recursive-descent parser in [src/parser.c](../src/parser.c), which implicitly behaves like a PDA via the C call stack.
- Identified that the lexer in [src/lexer.c](../src/lexer.c) feeds a token array that the parser consumes with `match()` and `parse_index`, matching the PDA input-tape model.
- Added a concise section to [task.md](../task.md) explicitly mapping PDA concepts (stack, input, recovery states) to the project’s parser functions so future work can extend the grammar confidently.

## Notes for Future Work
- If grammar grows, keep the recursive structure in [src/parser.c](../src/parser.c) to preserve PDA semantics and reuse `panicRecovery()` for synchronization.
- Consider documenting any new non-terminals next to their functions to keep the grammar-to-code mapping clear for classmates.
