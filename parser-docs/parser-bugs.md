# Parser Bugs

These are the parser defects we have confirmed by tracing control flow in the grammar functions (see linked line ranges). They are the ones that currently surface as hard syntax failures or bad recovery in typical inputs.

## Known issues

- **Standalone blocks rejected.** `parseStatement` routes `{` to `parseCompoundStatement`, but `parseCompoundStatement` never handles `LCBRACE` because the branch is commented out, so bare blocks fall into panic recovery instead of parsing their contents. [src/parser.c](src/parser.c#L248-L282)
- **`REPEATFOR` token unsupported.** The token is defined and printable, but no grammar branch ever accepts it, so any `repeatfor` loop is a syntax error. [src/parser.c](src/parser.c#L111-L116)
- **Prefix unary operators double-parse operands.** `parseUnary` recurses after consuming a prefix operator and then still calls `parsePostfix`, so an expression like `-x` parses `x` once in the recursive call and then immediately tries to parse another primary, producing spurious errors/consuming the next token. [src/parser.c](src/parser.c#L535-L546)
- **`NOTHING` literal unrecognized.** The token is declared, but `parsePrimary` never accepts it, so the literal cannot appear in expressions. [src/parser.c](src/parser.c#L140-L146) [src/parser.c](src/parser.c#L560-L584)
- **Identifier statements must be assignments.** `parseSimpleStatement` dispatches any leading identifier to `parseAssStmnt`, which immediately requires `=`. Valid expression or call statements such as `foo()`/`bar++` therefore fail even though the expression grammar underneath can parse them. [src/parser.c](src/parser.c#L284-L304) [src/parser.c](src/parser.c#L394-L404)
- **Declaration proceeds after datatype error.** `parseDecStmnt` ignores the success/failure of `parseDataType` and continues into `parseDecList`, cascading errors instead of stopping when the type is missing or invalid. [src/parser.c](src/parser.c#L307-L347) [src/parser.c](src/parser.c#L363-L379)

## Possible solutions (critical take)

- Standalone blocks: re-enable `LCBRACE` handling in `parseCompoundStatement` (or redirect `{` to a dedicated block parser) and ensure its panic-recovery token set matches `parseStatement` to avoid new desyncs when nested blocks appear.
- `REPEATFOR` token: add a grammar branch that mirrors the existing repeat loop semantics (condition + body, or iterator form if intended) and wire it into `parseIteration`. Confirm scanner delivers the right token and that precedence with `repeat`/`while` is unambiguous.
- Prefix unary operand parsing: make `parseUnary` either consume the prefix op and then call `parseUnary` (not `parsePostfix`), or consume op then call `parsePostfix` directly, but never both. Add tests for `-x`, `!f()`, and chained prefix forms to catch regressions.
- `NOTHING` literal: accept the token in `parsePrimary`, map it to the AST node that other literals use, and verify it prints/emits correctly. Watch for collisions if `NOTHING` was also intended as an identifier in older inputs.
- Identifier statements: in `parseSimpleStatement`, only route to `parseAssStmnt` when the lookahead is `ASSIGN`; otherwise fall through to a general expression statement (call, increment, etc.). Confirm this does not mask declaration lookahead (e.g., identifier followed by type markers).
- Declaration after datatype error: have `parseDecStmnt` bail out when `parseDataType` fails and synchronize to the next semicolon/braced block before touching `parseDecList`. Avoid double-emitting diagnostics on the same missing type.
