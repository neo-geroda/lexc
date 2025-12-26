# PDA Transition Table (Parser View)

This table maps the current recursive-descent parser in `src/parser.c` to a pushdown automaton view. Each nonterminal corresponds to a PDA state; function calls are implicit pushes; returns are pops. Error handling follows the existing `panicRecovery()` (skip to `;` or `}` or EOF).

## Legend
- `State`: parser function / nonterminal
- `Input`: lookahead token category
- `Action`: consume (`match`), transition (call), loop, or error recovery
- `Push/Pop`: implicit call/return on the C call stack

## Transitions

| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| Program | any | go `StatementList` | push `StatementList`, return then pop |
| StatementList | `EOF`, `RCBRACE` | stop (return) | pop |
| StatementList | otherwise | go `Statement`; after, if `;` then consume; loop | push `Statement`, pop |
| Statement | `IF` | go `Compound` | push, pop |
| Statement | `REPEAT` | go `Compound` | push, pop |
| Statement | `LCBRACE` | go `Compound` (currently only IF/REPEAT handled) | push, pop |
| Statement | otherwise | go `Simple`; expect `;` | push, pop |
| Compound | `IF` | `ConditionalStmnt` | push, pop |
| Compound | `REPEAT` | `IterStmnt` | push, pop |
| Simple | type keyword (`NUMBER` `DECIMAL` `TEXT` `BOOL` `SYMBOL` `LIST`) | `DecStmnt` | push, pop |
| Simple | `IDENT` | `AssStmnt` | push, pop |
| Simple | `DISPLAY` | `OutputStmnt` | push, pop |
| Simple | `STOP`/`CONTINUE` | `JumpStmnt` | push, pop |
| DecStmnt | datatype | `DataType`; `DecList` | push `DataType`; push `DecList`; pop each |
| DecList | any | `DecItem` then repeat on `,` | push `DecItem`; loop |
| DecItem | `IDENT` | consume; optional `DecItemSuffix` | push `DecItemSuffix`, pop |
| DecItemSuffix | `=` | consume; if next `GET` then `InputStmnt` else `Expr` | push selected, pop |
| InputStmnt | `GET` | match `GET ( DataType )` | push `DataType` |
| OutputStmnt | `DISPLAY` | match `DISPLAY ( Expr )` | push `Expr` |
| AssStmnt | `IDENT` | match `IDENT =`; then `GET` branch -> `InputStmnt` else `Expr` | push selected |
| ConditionalStmnt | `IF` | `IF ( Expr ) { StatementList }`; then `ElifList`; `ElseOpt` | push sequences |
| ElifList | `ELIF` | repeat `ELIF ( Expr ) { StatementList }` | push per block |
| ElifList | other | epsilon (return) | pop |
| ElseOpt | `ELSE` | `ELSE { StatementList }` | push `StatementList` |
| ElseOpt | other | epsilon (return) | pop |
| IterStmnt | `REPEAT` | `REPEAT ( Expr ) { StatementList }` | push `Expr`, `StatementList` |
| JumpStmnt | `STOP` or `CONTINUE` | consume token | pop |
| Expr | any expr start | go `BoolExpr` | push `BoolExpr` |
| BoolExpr | any expr start | `RelExpr` ( `OR` `RelExpr` )* | push `RelExpr`; loop |
| RelExpr | any expr start | `AddExpr` [ relop `AddExpr` ] | push `AddExpr`; optional loop |
| AddExpr | any expr start | `Term` ( +/− `Term` )* | push `Term`; loop |
| Term | any expr start | `Power` ( */ // % `Power` )* | push `Power`; loop |
| Power | any expr start | `Unary` ( ** `Unary` )* | push `Unary`; loop |
| Unary | prefix op (+ − ++ -- NOT)* | consume ops then `Postfix` | push `Postfix` |
| Postfix | primary | `Primary` then (post++/post--) | push `Primary`; loop |
| Primary | literal (`NUM_LIT`, `DEC_LIT`, `SYM_LIT`, `TEXT_LIT`, `TRUE`, `FALSE`) | consume | pop |
| Primary | `IDENT` | consume | pop |
| Primary | `LEFT_PAREN` | consume, `Expr`, consume `RIGHT_PAREN` | push `Expr` |
| Any state | unexpected | `panicRecovery` → skip until `;` or `}` or EOF, then return | pop |

## Notes
- Push/pop is implicit via function calls; no explicit stack array is used.
- Synchronization tokens for recovery are `;`, `}`, and EOF.
- The table mirrors the current code; adding grammar requires adding rows and adjusting recovery sets.
