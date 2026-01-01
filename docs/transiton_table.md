This document models the recursive-descent parser (`parser.c`) as a **Pushdown Automaton (PDA)**.
Each parser function corresponds to a PDA state.
Function calls imply **stack push**, and returns imply **stack pop**.

---

## Legend

* **State** – Conceptual PDA state / parser function
* **Input Token** – `current_token_parse.type` (as consumed by `parser.c`)
* **Action / Transition** – Parser behavior
* **Stack** – Implicit call-stack effect

---

## Program and Statement List

| State          | Input Token            | Action / Transition     | Stack            |
| -------------- | ---------------------- | ----------------------- | ---------------- |
| PROGRAM        | any                    | `parseStatementList()`  | push             |
| STATEMENT_LIST | `EOF_TOKEN`, `RCBRACE` | ε (return)              | pop              |
| STATEMENT_LIST | otherwise              | `parseStatement()` loop | push `STATEMENT` |

---

## Statement Dispatch

| State     | Input Token | Action / Transition                       | Stack |
| --------- | ----------- | ----------------------------------------- | ----- |
| STATEMENT | `IF`        | `parseConditionalStmnt()`                 | push  |
| STATEMENT | `REPEAT`    | `parseIterStmnt()`                        | push  |
| STATEMENT | otherwise   | `parseSimpleStatement()` then `SEMICOLON` | push  |

---

## Simple Statements

| State       | Input Token                                           | Action / Transition  | Stack |
| ----------- | ----------------------------------------------------- | -------------------- | ----- |
| SIMPLE_STMT | `NUMBER`, `DECIMAL`, `TEXT`, `BOOL`, `SYMBOL`, `LIST` | `parseDecStmnt()`    | push  |
| SIMPLE_STMT | `IDENT`                                               | `parseAssStmnt()`    | push  |
| SIMPLE_STMT | `DISPLAY`                                             | `parseOutputStmnt()` | push  |
| SIMPLE_STMT | `STOP`, `CONTINUE`                                    | `parseJumpStmnt()`   | pop   |

---

## Declarations

| State           | Input Token           | Action / Transition                  | Stack |
| --------------- | --------------------- | ------------------------------------ | ----- |
| DEC_STMT        | datatype token        | `parseDataType()` → `parseDecList()` | push  |
| DEC_LIST        | any                   | `DEC_ITEM ( COMMA DEC_ITEM )*`       | loop  |
| DEC_ITEM        | `IDENT`               | consume `IDENT` → `DEC_ITEM_SUFFIX`  | push  |
| DEC_ITEM_SUFFIX | `ASSIGN_OP`           | consume `=` then branch              | push  |
| DEC_ITEM_SUFFIX | `ASSIGN_OP GET`       | `parseInputStmnt()`                  | push  |
| DEC_ITEM_SUFFIX | `ASSIGN_OP` otherwise | `parseExpr()`                        | push  |
| DEC_ITEM_SUFFIX | otherwise             | ε                                    | pop   |

---

## Assignment Statement

| State       | Input Token | Action / Transition    | Stack   |      |
| ----------- | ----------- | ---------------------- | ------- | ---- |
| ASSIGN_STMT | `IDENT`     | `IDENT ASSIGN_OP ( GET | EXPR )` | push |

---

## Input and Output

| State       | Input Token | Action / Transition                   | Stack           |
| ----------- | ----------- | ------------------------------------- | --------------- |
| INPUT_STMT  | `GET`       | `GET LEFT_PAREN DataType RIGHT_PAREN` | push `DataType` |
| OUTPUT_STMT | `DISPLAY`   | `DISPLAY LEFT_PAREN EXPR RIGHT_PAREN` | push `EXPR`     |

---

## Conditional Statements

| State            | Input Token | Action / Transition                     | Stack |
| ---------------- | ----------- | --------------------------------------- | ----- |
| CONDITIONAL_STMT | `IF`        | `IF ( EXPR ) { STATEMENT_LIST }`        | push  |
| ELIF_LIST        | `ELIF`      | `ELIF ( EXPR ) { STATEMENT_LIST }` loop | loop  |
| ELSE_OPT         | `ELSE`      | `{ STATEMENT_LIST }` optional           | push  |
| ELSE_OPT         | otherwise   | ε                                       | pop   |

---

## Iterative Statement

| State     | Input Token | Action / Transition                  | Stack |
| --------- | ----------- | ------------------------------------ | ----- |
| ITER_STMT | `REPEAT`    | `REPEAT ( EXPR ) { STATEMENT_LIST }` | push  |

---

## Expressions (Operator-Precedence PDA)

| State     | Input Token                                                  | Action / Transition            | Stack                 |                  |                     |                    |      |
| --------- | ------------------------------------------------------------ | ------------------------------ | --------------------- | ---------------- | ------------------- | ------------------ | ---- |
| EXPR      | expression start                                             | `parseBoolExpr()`              | push                  |                  |                     |                    |      |
| BOOL_EXPR | expression start                                             | `REL_EXPR ( OR_OP REL_EXPR )*` | loop                  |                  |                     |                    |      |
| REL_EXPR  | expression start                                             | `ADD_EXPR ( relop ADD_EXPR )?` | optional              |                  |                     |                    |      |
| ADD_EXPR  | expression start                                             | `TERM ( ADDITION_OP            | SUBTRACT_OP TERM )*`  | loop             |                     |                    |      |
| TERM      | expression start                                             | `POWER ( MULTIPLY_OP           | DIVIDE_OP             | FLOORDIV_OP      | MODULO_OP POWER )*` | loop               |      |
| POWER     | expression start                                             | `UNARY ( EXPONENT_OP UNARY )*` | loop                  |                  |                     |                    |      |
| UNARY     | prefix ops                                                   | `( +                           | -                     | PRE_INCREMENT_OP | PRE_DECREMENT_OP    | NOT_OP )* POSTFIX` | push |
| POSTFIX   | primary                                                      | `PRIMARY ( POST_INCREMENT_OP   | POST_DECREMENT_OP )*` | loop             |                     |                    |      |
| PRIMARY   | `NUM_LIT`, `DEC_LIT`, `SYM_LIT`, `TEXT_LIT`, `TRUE`, `FALSE` | consume literal                | pop                   |                  |                     |                    |      |
| PRIMARY   | `IDENT`                                                      | consume `IDENT`                | pop                   |                  |                     |                    |      |
| PRIMARY   | `LEFT_PAREN`                                                 | `( EXPR )`                     | push                  |                  |                     |                    |      |

---

## Error Recovery (Global)

| State | Input Token | Action / Transition                                                | Stack          |
| ----- | ----------- | ------------------------------------------------------------------ | -------------- |
| ANY   | unexpected  | `panicRecovery()` → skip until `SEMICOLON`, `RCBRACE`, `EOF_TOKEN` | pop until sync |

---

## Notes

* All tokens and transitions now **directly match `parser.c` behavior**, the parser implementation.
* Stack behavior reflects **actual recursive calls**, not grammar symbols.
* `SEMICOLON` handling is external to simple statements, matching `parseStatement()`.
* Expression precedence exactly mirrors function nesting in `parser.c`.
