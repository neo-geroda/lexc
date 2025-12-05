## Changes to `parseDecItemSuffix`

The `parseDecItemSuffix` function was modified to introduce **non-determinism** in the grammar for declaration statements. Specifically, the following changes were made:

1. **Added Backtracking**:
   - When encountering a comma `,`, the parser now attempts to parse the `, <DECL_ITEM>` alternative.
   - If parsing fails, the parser backtracks to its previous state and treats the comma as part of `<DECL_LIST_TAIL>` instead.

2. **Grammar Adjustments**:
   - The `<DECL_ITEM_SUFFIX>` production was updated to include a new alternative:
     ```
     <DECL_ITEM_SUFFIX> ::= "=" <EXPR>
                          | "," <DECL_ITEM>
                          | ε
     ```
   - This introduces ambiguity because the comma `,` can now belong to either `<DECL_ITEM_SUFFIX>` or `<DECL_LIST_TAIL>`.

### Why This Was Done

#### 1. **Introduce Non-Determinism**
The goal was to make the grammar non-deterministic, as per the requirements. This was achieved by creating a situation where the parser cannot decide between two alternatives (`<DECL_ITEM_SUFFIX>` and `<DECL_LIST_TAIL>`) using a single token lookahead.

#### 2. **Test Parser Robustness**
By introducing non-determinism, the parser's ability to handle ambiguous constructs is tested. This ensures that the parser can:
- Handle real-world grammars that are often ambiguous.
- Use backtracking to resolve conflicts.

#### 3. **Maintain `<EXPR>` Integrity**
The `<EXPR>` production was left untouched, as requested. All changes were confined to the declaration-related productions.

### What Problem Does This Solve?
The changes allow the parser to handle ambiguous declarations like:
```c
int a, b, c = 5;
```
In this example:
- `a, b` could be part of `<DECL_LIST_TAIL>`.
- `a` could have a `<DECL_ITEM_SUFFIX>` of `, b`.

The parser can now handle both interpretations, making it more versatile.

### How It Works
1. **Backtracking**:
   - The parser saves its state (e.g., `parse_index`) before attempting the `, <DECL_ITEM>` alternative.
   - If parsing fails, the state is restored, and the parser tries the `ε` alternative instead.

2. **Epsilon Production**:
   - If no valid alternative is found, the parser defaults to the `ε` production, effectively doing nothing.

### Future Considerations
1. **Performance**:
   - Backtracking can be computationally expensive. If performance becomes an issue, consider refactoring the grammar to eliminate ambiguity.

2. **Testing**:
   - Ensure thorough testing with ambiguous and edge-case declarations to verify the parser's behavior.

3. **Extensibility**:
   - The backtracking mechanism can be reused for other ambiguous constructs in the grammar.

---

### Example Test Cases

#### 1. Simple Declaration
```c
int x;
```
- Expected: Parsed as `<DATA_TYPE> int` and `<DECL_ITEM> x`.

#### 2. Multiple Declarations
```c
int a, b, c;
```
- Expected: Parsed as `<DATA_TYPE> int` and `<DECL_LIST>` with multiple `<DECL_ITEM>`s.

#### 3. Ambiguous Declaration
```c
int a, b, c = 5;
```
- Expected: The parser should handle both interpretations:
  - `a, b` as part of `<DECL_LIST_TAIL>`.
  - `a` with `, b` as part of `<DECL_ITEM_SUFFIX>`.

#### 4. Complex Declaration
```c
int x = 10, y, z = 20;
```
- Expected: The parser should correctly handle mixed `<DECL_ITEM_SUFFIX>` alternatives.