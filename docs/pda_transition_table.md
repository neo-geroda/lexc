# PDA Transition Table (Parser View)

This table maps the current recursive-descent parser in `src/parser.c` to a pushdown automaton view. Each nonterminal corresponds to a PDA state; function calls are implicit pushes; returns are pops. Error handling follows the existing `panicRecovery()` (skip to `;` or `}` or EOF).

## Legend
- `State`: parser function / nonterminal
- `Input`: lookahead token category
- `Action`: consume (`match`), transition (call), loop, or error recovery
- `Push/Pop`: implicit call/return on the C call stack

## Transitions (updated to the current CFG)

### Program / Functions / Lists
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| Program | any | go `ProgramItem`; then `ProgramTail` | push `ProgramItem`, then `ProgramTail`; pop on return |
| ProgramTail | `EOF` | return | pop |
| ProgramTail | otherwise | go `ProgramItem`; loop | push `ProgramItem`, then repeat |
| ProgramItem | datatype + `IDENT` | if next is `LEFT_PAREN` → `FunctionDef`; else `Statement` then expect `;` | push selected, pop |
| ProgramItem | other stmt start | `Statement` then expect `;` | push `Statement`, pop |
| StatementList | `EOF`, `RCBRACE`, `END` | return | pop |
| StatementList | otherwise | go `Statement`; expect `;`; loop | push `Statement`, pop |

### Statements
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| Statement | datatype | `DeclarationStmnt` | push, pop |
| Statement | `GET` | `InputStmnt` | push, pop |
| Statement | `DISPLAY` | `OutputStmnt` | push, pop |
| Statement | `IDENT` | `AssStmnt` | push, pop |
| Statement | `IF` | `ConditionalStmnt` | push, pop |
| Statement | `REPEAT` | `Repeat` | push, pop |
| Statement | `REPEATFOR` | `RepeatFor` | push, pop |
| Statement | `RETURN`/`BREAK`/`CONTINUE` | `JumpStmnt` | push, pop |

### Functions and Parameters
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| FunctionDef | datatype | match datatype, `IDENT`, `LEFT_PAREN`; `OptParamDefs`; `RIGHT_PAREN`; `LCBRACE`; `StatementList`; `RCBRACE` | push `OptParamDefs`, `StatementList`; pop each |
| OptParamDefs | `IDENT` | `IdList` | push `IdList`, pop |
| OptParamDefs | `RIGHT_PAREN` | epsilon | pop |
| IdList | `IDENT` | consume ident; `IdListTail` | push `IdListTail`, pop |
| IdListTail | `COMMA` | consume, ident, loop | push `IdListTail`, pop |
| IdListTail | other | epsilon | pop |

### Declarations, I/O, Assignment
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| DeclarationStmnt | datatype | `DataType`; match `IDENT`; `DeclSuffix` | push `DataType`, `DeclSuffix`; pop |
| DeclSuffix | `ASSIGN_OP` | consume; `Expr` | push `Expr`, pop |
| DeclSuffix | other | epsilon | pop |
| InputStmnt | `GET` | match `GET ( DataType )` | push `DataType`, pop |
| OutputStmnt | `DISPLAY` | match `DISPLAY ( Expr )` | push `Expr`, pop |
| AssStmnt | `IDENT` | match `IDENT =`; if next `GET` → `InputStmnt` else `Expr` | push selected |

### Conditionals and Iteration
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| ConditionalStmnt | `IF` | `IF ( Expr ) { StatementList }` then `ElifList` then `ElseOpt` | push each subcall |
| ElifList | `ELIF` | repeat `ELIF ( Expr ) { StatementList }` | push per block |
| ElifList | other | epsilon | pop |
| ElseOpt | `ELSE` | `ELSE { StatementList }` | push `StatementList`, pop |
| ElseOpt | other | epsilon | pop |
| Repeat | `REPEAT` | `REPEAT ( Expr ) { StatementList } END` | push `Expr`, `StatementList`; pop |
| RepeatFor | `REPEATFOR` | `REPEATFOR IDENT = Expr TO Expr { StatementList } END` | push `Expr` (init, bound), `StatementList`; pop |

### Jumps
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| JumpStmnt | `RETURN` | consume; optional `Expr` | push `Expr` if present |
| JumpStmnt | `BREAK`/`CONTINUE` | consume token | pop |

### Expressions (precedence chain)
| State | Input | Action | Push/Pop |
| --- | --- | --- | --- |
| Expr | any expr start | go `BoolExpr` | push `BoolExpr` |
| BoolExpr | any expr start | `AndTerm` then `OrTail` | push `AndTerm`; loop via `OrTail` |
| OrTail | `OR_OP` | consume; `AndTerm`; loop | push `AndTerm` |
| OrTail | other | epsilon | pop |
| AndTerm | any expr start | `NotTerm` then `AndTail` | push `NotTerm`; loop |
| AndTail | `AND_OP` | consume; `NotTerm`; loop | push `NotTerm` |
| AndTail | other | epsilon | pop |
| NotTerm | `NOT_OP` | consume; recurse `NotTerm` | push `NotTerm` |
| NotTerm | other | `RelExpr` | push `RelExpr` |
| RelExpr | any expr start | `AddExpr`; optional relop + `AddExpr` | push `AddExpr`; optional repeat |
| AddExpr | any expr start | `Term` ( +/- `Term` )* | push `Term`; loop |
| Term | any expr start | `Power` ( */ // % `Power` )* | push `Power`; loop |
| Power | any expr start | `Unary` ( ^ `Unary` )* | push `Unary`; loop |
| Unary | prefix op (+ − ++ -- NOT)* | consume ops then `Postfix` | push `Postfix` |
| Postfix | primary | `Primary` then (post++/post--) | push `Primary`; loop |
| Primary | literal (`NUM_LIT`, `DEC_LIT`, `SYM_LIT`, `TEXT_LIT`, `TRUE`, `FALSE`) | consume | pop |
| Primary | `IDENT` | consume; optional `IdTail` | push `IdTail`, pop |
| Primary | `LEFT_PAREN` | consume, `Expr`, consume `RIGHT_PAREN` | push `Expr` |
| IdTail | `LEFT_PAREN` | parse `OptParams`; consume `RIGHT_PAREN` | push `OptParams` |
| IdTail | other | epsilon | pop |
| OptParams | expr start | `ParamList` | push `ParamList` |
| OptParams | `RIGHT_PAREN` | epsilon | pop |
| ParamList | expr start | `Expr`; `ParamTail` | push `Expr`, `ParamTail` |
| ParamTail | `COMMA` | consume; `Expr`; loop | push `Expr`, `ParamTail` |
| ParamTail | other | epsilon | pop |
| Any state | unexpected | `panicRecovery` → skip until `;`, `}`, `END`, or EOF, then return | pop |

## Notes
- Push/pop is implicit via function calls; no explicit stack array is used.
- Synchronization tokens for recovery are `;`, `}`, and EOF.
- The table mirrors the current code; adding grammar requires adding rows and adjusting recovery sets.
