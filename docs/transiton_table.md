# PDA Transition Table (Token-Based, Code-Aligned)

This document models the recursive-descent parser (`parser.c`) as a **Pushdown Automaton (PDA)**.
Each parser function corresponds to a PDA state.  
Function calls imply **stack push**, and returns imply **stack pop**.  
---

## Legend

- **State** – Conceptual PDA state / grammar role
- **Input Token** – `current_token_parse.type` (from `tokens.h`)
- **Action** – Parser behavior (`match`, transition, loop)
- **Stack** – Implicit call stack behavior

---

## Program and Statements

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| START | any | transition to `STATEMENT_LIST` | push |
| STATEMENT_LIST | `EOF_TOKEN`, `RCBRACE` | ε (return) | pop |
| STATEMENT_LIST | otherwise | go to `STATEMENT`, then loop | push `STATEMENT` |
| STATEMENT | `IF` | go to `CONDITIONAL_STMT` | push |
| STATEMENT | `REPEAT` | go to `ITER_STMT` | push |
| STATEMENT | otherwise | go to `SIMPLE_STMT`, expect `SEMICOLON` | push |

---

## Simple Statements

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| SIMPLE_STMT | `NUMBER`, `DECIMAL`, `TEXT`, `BOOL`, `SYMBOL`, `LIST` | go to `DECL_STMT` | push |
| SIMPLE_STMT | `IDENT` | go to `ASSIGN_STMT` | push |
| SIMPLE_STMT | `DISPLAY` | go to `OUTPUT_STMT` | push |
| SIMPLE_STMT | `STOP`, `CONTINUE` | consume token | pop |

---

## Declarations and Assignment

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| DECL_STMT | datatype token | `match(DataType)` → `DECL_LIST` | push |
| DECL_LIST | any | `DECL_ITEM` ( `COMMA` `DECL_ITEM` )* | loop |
| DECL_ITEM | `IDENT` | consume `IDENT`, optional suffix | push |
| DECL_ITEM_SUFFIX | `ASSIGN_OP` | consume `ASSIGN_OP`, then branch | push |
| DECL_ITEM_SUFFIX | `GET` | go to `INPUT_STMT` | push |
| DECL_ITEM_SUFFIX | otherwise | go to `EXPR` | push |
| ASSIGN_STMT | `IDENT` | `IDENT ASSIGN_OP ( INPUT_STMT | EXPR )` | push |

---

## Input and Output

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| INPUT_STMT | `GET` | `GET LEFT_PAREN DataType RIGHT_PAREN` | push `DataType` |
| OUTPUT_STMT | `DISPLAY` | `DISPLAY LEFT_PAREN EXPR RIGHT_PAREN` | push `EXPR` |

---

## Control Structures

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| CONDITIONAL_STMT | `IF` | `IF LEFT_PAREN EXPR RIGHT_PAREN LCBRACE STATEMENT_LIST RCBRACE` | push |
| CONDITIONALIONAL_STMT | `ELIF` | repeat ELIF blocks | loop |
| CONDITIONAL_STMT | `ELSE` | optional ELSE block | push |
| ITER_STMT | `REPEAT` | `REPEAT LEFT_PAREN EXPR RIGHT_PAREN LCBRACE STATEMENT_LIST RCBRACE` | push |

---

## Expressions (Operator-Precedence PDA)

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| EXPR | expression start | go to `BOOL_EXPR` | push |
| BOOL_EXPR | expression start | `REL_EXPR ( OR_OP REL_EXPR )*` | loop |
| REL_EXPR | expression start | `ADD_EXPR ( relop ADD_EXPR )?` | optional |
| ADD_EXPR | expression start | `TERM ( ADDITION_OP | SUBTRACT_OP TERM )*` | loop |
| TERM | expression start | `POWER ( MULTIPLY_OP | DIVIDE_OP | FLOORDIV_OP | MODULO_OP POWER )*` | loop |
| POWER | expression start | `UNARY ( EXPONENT_OP UNARY )*` | loop |
| UNARY | prefix ops | `( ADDITION_OP | SUBTRACT_OP | PRE_INCREMENT_OP | PRE_DECREMENT_OP | NOT_OP )* POSTFIX` | push |
| POSTFIX | primary | `PRIMARY ( POST_INCREMENT_OP | POST_DECREMENT_OP )*` | loop |
| PRIMARY | literal tokens | consume literal | pop |
| PRIMARY | `IDENT` | consume `IDENT` | pop |
| PRIMARY | `LEFT_PAREN` | `LEFT_PAREN EXPR RIGHT_PAREN` | push |

---

## Error Recovery (Global)

| State | Input Token | Action | Stack |
|-----|-----------|--------|-------|
| ANY | unexpected | `panicRecovery()` → skip until `SEMICOLON`, `RCBRACE`, or `EOF_TOKEN` | pop |

---

## Notes

- Stack operations are **implicit** via recursive function calls.
- Synchronization tokens for recovery are `SEMICOLON`, `RCBRACE`, and `EOF_TOKEN`.
- This PDA abstraction preserves operator precedence and control-flow semantics of the parser.
