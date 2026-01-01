This document models the recursive-descent parser (`parser.c`) as a **Pushdown Automaton (PDA)**.
Each parser function corresponds to a PDA state.
Function calls imply **stack push**, and returns imply **stack pop**.

---

## Program and Statement Control

| State          | Input Token Group      | Transition / Action                     | Stack            |
| -------------- | ---------------------- | --------------------------------------- | ---------------- |
| PROGRAM        | any                    | enter `STATEMENT_LIST`                  | push             |
| STATEMENT_LIST | `EOF_TOKEN`, `RCBRACE` | ε (return)                              | pop              |
| STATEMENT_LIST | otherwise              | `STATEMENT`, then loop                  | push `STATEMENT` |
| STATEMENT      | `IF`                   | go to `CONDITIONAL_STMT`                | push             |
| STATEMENT      | `REPEAT`               | go to `ITER_STMT`                       | push             |
| STATEMENT      | otherwise              | go to `SIMPLE_STMT`, expect `SEMICOLON` | push             |

---

## Simple Statements

| State       | Input Token Group                                                 | Transition / Action | Stack |
| ----------- | ----------------------------------------------------------------- | ------------------- | ----- |
| SIMPLE_STMT | data type (`NUMBER`, `DECIMAL`, `TEXT`, `BOOL`, `SYMBOL`, `LIST`) | `DECL_STMT`         | push  |
| SIMPLE_STMT | `IDENT`                                                           | `ASSIGN_STMT`       | push  |
| SIMPLE_STMT | `DISPLAY`                                                         | `OUTPUT_STMT`       | push  |
| SIMPLE_STMT | `STOP`, `CONTINUE`                                                | consume jump        | pop   |

---

## Control Structures

| State            | Input Token Group | Transition / Action                     | Stack |
| ---------------- | ----------------- | --------------------------------------- | ----- |
| CONDITIONAL_STMT | `IF`              | `IF ( EXPR ) { STATEMENT_LIST }`        | push  |
| ELIF_LIST        | `ELIF`            | `ELIF ( EXPR ) { STATEMENT_LIST }` loop | loop  |
| ELSE_OPT         | `ELSE`            | `{ STATEMENT_LIST }` optional           | push  |
| ITER_STMT        | `REPEAT`          | `REPEAT ( EXPR ) { STATEMENT_LIST }`    | push  |

---

## Declarations and Assignments

| State            | Input Token Group | Transition / Action          | Stack   |
| ---------------- | ----------------- | ---------------------------- | ------- |
| DECL_STMT        | data type         | `DataType → DECL_LIST`       | push    |
| DECL_LIST        | any               | `DECL_ITEM ( , DECL_ITEM )*` | loop    |
| DECL_ITEM        | `IDENT`           | consume `IDENT` → suffix     | push    |
| DECL_ITEM_SUFFIX | `=`               | `= GET ( DataType )`         | push    |
| DECL_ITEM_SUFFIX | `=`               | `= EXPR`                     | push    |
| DECL_ITEM_SUFFIX | otherwise         | ε                            | pop     |
| ASSIGN_STMT      | `IDENT`           | `IDENT = ( GET               | EXPR )`/ push |

---

## Expression PDA

| State     | Input Token Group                                                      | Transition / Action            | Stack                |
| --------- | ---------------------------------------------------------------------- | ------------------------------ | -------------------- |
| EXPR      | expression start                                                       | `BOOL_EXPR`                    | push                 |
| BOOL_EXPR | expression start                                                       | `REL_EXPR ( OR REL_EXPR )*`    | loop                 |
| REL_EXPR  | expression start                                                       | `ADD_EXPR ( relop ADD_EXPR )?` | optional             |
| ADD_EXPR  | any expr start                                                         | `Term ( +/− Term )*`           | push `Term`; loop    |
| TERM      | any expr start                                                         | `Power ( / // % Power )`       | push `Power`; loop   |
| POWER     | any expr start                                                         | `Unary ( ** Unary )*`          | push `Unary`; loop   |
| UNARY     | prefix op (+ − ++ -- NOT)*                                             | consume ops then `Postfix`     | push `Postfix`       |
| POSTFIX   | primary                                                                | `Primary then (post++/post--)` | push `Primary`; loop |
| PRIMARY   | literal (`NUM_LIT`, `DEC_LIT`, `TEXT_LIT`, `SYM_LIT`, `TRUE`, `FALSE`) | consume literal                | pop                  |
| PRIMARY   | `IDENT`                                                                | consume `IDENT`                | pop                  |
| PRIMARY   | `(`                                                                    | `( EXPR )`                     | push                 |

---

## Error Handling

| State | Input Token Group | Transition / Action                               | Stack          |
| ----- | ----------------- | ------------------------------------------------- | -------------- |
| ANY   | invalid           | skip until `SEMICOLON`, `RCBRACE`, or `EOF_TOKEN` | pop until sync |

---

## Notes

* Token groups are clustered **by diagram responsibility**, not by header definitions.
* The PDA reflects **actual control flow in `parser.c`**, with abstraction only where behavior is identical.
* Operator precedence is preserved by state ordering, not by grammar rules.
