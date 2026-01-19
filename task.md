## 2. Are Parsing and PDAs Necessary?

**Short Answer:** Yes.

**Long Answer:** If you have a CFG, you **must** have a parser to use it. In Automata Theory, the machine that executes a Context-Free Grammar is a **Pushdown Automaton (PDA)**.

> However, as a software engineer, you rarely code a "raw" PDA (with states $q_0, q_1$ and stack operations like $pop(Z)$) unless you are building a compiler from scratch.

Instead, you will likely implement a **Parser**, which is the practical software implementation of a PDA.

* **If you write a Recursive Descent Parser:** You are implicitly using the "Call Stack" of your programming language to handle the memory. This *is* a PDA in disguise.
* **If you write a Shift-Reduce Parser:** You will explicitly create a `Stack` data structure to hold tokens. This is also a PDA.

### How This Applies to the LexC Project

- The current parser in [src/parser.c](src/parser.c) is a recursive-descent style driver. Each non-terminal (for example, `parseExpr()` and `parseStatementList()`) becomes a C function; the C call stack serves as the PDA stack.
- The "input tape" for the PDA is the token array produced by the lexer in [src/lexer.c](src/lexer.c) and streamed via `match()`/`parse_index`.
- Error handling uses `panicRecovery()` to skip to safe synchronization tokens (like `;` or `}`), which mirrors a PDA moving to a recovery state rather than halting immediately.
- To extend the language, add or adjust grammar functions in [src/parser.c](src/parser.c) while keeping the recursive structure; no explicit stack is needed unless you switch to a shift-reduce approach.