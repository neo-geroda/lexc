# Syntax Tree Generation

The compiler generates and displays an Abstract Syntax Tree (AST) for .lexc programs, following the formal EBNF grammar specification defined in the PPL Project Proposal Group 6.md research paper.

## Design Philosophy

### Why EBNF-Aligned Node Types?

The syntax tree implementation was updated to **precisely match the formal grammar** defined in our language specification. This alignment ensures:

1. **Academic Rigor**: The parse tree directly reflects the EBNF grammar rules (G = {V, T, P, S})
2. **Traceability**: Each node type corresponds to a non-terminal symbol in the grammar
3. **Consistency**: Output notation uses angle brackets for non-terminals (`<LexCProgram>`, `<STMNT_LIST>`) matching formal notation
4. **Expression Hierarchy**: Implements the complete operator precedence chain from the grammar

### Grammar Structure

Our AST follows this production hierarchy from the formal grammar:

```
<LexCProgram> → <STMNT_LIST>
<STMNT_LIST> → <STMNT> | <STMNT> <STMNT_LIST>
<STMNT> → <SIMPLE_STMNT> | <COMPOUND_STMNT>
<SIMPLE_STMNT> → <DECLARATION_STMNT> | <ASSMNT_STMNT> | <INPUT_STMNT> | <OUTPUT_STMNT> | <JUMP_STMNT>
<COMPOUND_STMNT> → <CONDITIONAL_STMNT> | <ITER_STMNT>
```

**Expression Precedence Hierarchy** (lowest to highest):
```
<EXPR> → <BOOL_EXPR>
<BOOL_EXPR> → <AND_TERM> (or <AND_TERM>)*
<AND_TERM> → <NOT_TERM> (and <NOT_TERM>)*
<NOT_TERM> → not <REL_EXPR> | <REL_EXPR>
<REL_EXPR> → <ADDITIVE_EXPR> [relop <ADDITIVE_EXPR>]
<ADDITIVE_EXPR> → <TERM> [(+|-) <TERM>]*
<TERM> → <POWER> [(*|/|//|%) <POWER>]*
<POWER> → <UNARY_EXPR> [^ <UNARY_EXPR>]*
<UNARY_EXPR> → [+|-|++|--|not] <POSTFIX_EXPR>
<POSTFIX_EXPR> → <PRIMARY> [++|--]*
<PRIMARY> → id | literal | ( <EXPR> )
```

## Usage

Compile and run the compiler on any .lexc file:

```bash
gcc -Iinclude -g -Wall src/main.c src/lexer.c src/parser.c src/symbol_table.c src/token_stream.c src/token_list.c src/lookupkeywords.c -o compiler.exe

.\compiler.exe tests\test.lexc
```

## Output Format

The syntax tree uses **EBNF notation** with angle brackets for non-terminals and lowercase for terminals:

```
<LexCProgram>
  <STMNT_LIST>
    <DECLARATION_STMNT>(NUMBER)
      id(k)
    <ASSMNT_STMNT>
      id(y)
      <INPUT_STMNT>(GET)
        <DATA_TYPE>(DECIMAL)
    <OUTPUT_STMNT>(DISPLAY)
      id(k)
```

## Node Types (EBNF-Aligned)

### Program Structure
- **`<LexCProgram>`**: Root node representing the entire program
- **`<STMNT_LIST>`**: Collection of statements

### Statement Types
- **`<DECLARATION_STMNT>(type)`**: Variable declaration with data type (NUMBER, DECIMAL, TEXT, BOOL, SYMBOL)
- **`<ASSMNT_STMNT>`**: Assignment statement
- **`<CONDITIONAL_STMNT>(if)`**: Conditional statement with if/elif/else branches
- **`elif(elif)`**: Elif clause in conditional
- **`else(else)`**: Else clause in conditional
- **`<REPEAT>(repeat)`**: Iteration/loop statement
- **`<INPUT_STMNT>(GET)`**: Input statement using get() function
- **`<OUTPUT_STMNT>(DISPLAY)`**: Output statement using display() function
- **`<JUMP_STMNT>`**: Control flow jump (continue/stop)

### Expression Operators (by precedence)
- **`<LOGICAL_OP>(or)`**: Boolean OR operation
- **`<LOGICAL_OP>(and)`**: Boolean AND operation  
- **`<COMPARE_OP>(==|!=|<|>|<=|>=)`**: Relational comparison operators
- **`<ADDITIVE_OP>(+|-)`**: Addition and subtraction
- **`<MULTIPLICATIVE_OP>(*|/|//|%)`**: Multiplication, division, floor division, modulo
- **`<POWER>(^)`**: Exponentiation
- **`<UNARY_EXPR>(+|-|++|--|not)`**: Unary operations (prefix)
- **`<POSTFIX_EXPR>(++|--)`**: Postfix increment/decrement

### Terminals
- **`id(name)`**: Identifier/variable reference
- **`<PRIMARY>(value)`**: Primary expression - literal value or identifier
  - Number literals: `<PRIMARY>(42)`
  - Decimal literals: `<PRIMARY>(3.14)`
  - Text literals: `<PRIMARY>("hello")`
  - Boolean literals: `<PRIMARY>(true)`, `<PRIMARY>(false)`
  - Symbol literals: `<PRIMARY>(:symbol)`
- **`<DATA_TYPE>(type)`**: Data type specification (NUMBER, DECIMAL, TEXT, BOOL, SYMBOL)

### Expression Hierarchy Nodes
- **`<EXPR>`**: Top-level expression
- **`<BOOL_EXPR>`**: Boolean expression with OR operations
- **`<AND_TERM>`**: Boolean expression with AND operations
- **`<NOT_TERM>`**: Negation expression
- **`<REL_EXPR>`**: Relational expression
- **`<ADDITIVE_EXPR>`**: Additive expression
- **`<TERM>`**: Multiplicative term
- **`<POWER>`**: Exponentiation expression
- **`<UNARY_EXPR>`**: Unary expression
- **`<POSTFIX_EXPR>`**: Postfix expression
- **`<PRIMARY>`**: Primary expression (leaf node)

## Example

For the program:
```
number k = 10;
display(k);
```

The syntax tree generated is:
```
<LexCProgram>
  <STMNT_LIST>
    <DECLARATION_STMNT>(NUMBER)
      id(k)
        <PRIMARY>(10)
    <OUTPUT_STMNT>(DISPLAY)
      id(k)
```

### Complex Expression Example

For the program with arithmetic operations:
```
number my_age = (4 * 5) + 1;
```

The syntax tree shows the operator precedence hierarchy:
```
<LexCProgram>
  <STMNT_LIST>
    <DECLARATION_STMNT>(NUMBER)
      id(my_age)
        <ADDITIVE_OP>(+)
          <MULTIPLICATIVE_OP>(*)
            <PRIMARY>(4)
            <PRIMARY>(5)
          <PRIMARY>(1)
```

Note how multiplication binds tighter than addition, correctly reflecting the expression precedence.

### Conditional Statement Example

For an if/elif/else structure:
```
if (x > 5) {
    display("greater");
} elif (x == 5) {
    display("equal");
} else {
    display("less");
}
```

The syntax tree:
```
<LexCProgram>
  <STMNT_LIST>
    <CONDITIONAL_STMNT>(if)
      <COMPARE_OP>(>)
        <PRIMARY>(x)
        <PRIMARY>(5)
      <STMNT_LIST>
        <OUTPUT_STMNT>(DISPLAY)
          <PRIMARY>("greater")
      elif(elif)
        <COMPARE_OP>(==)
          <PRIMARY>(x)
          <PRIMARY>(5)
        <STMNT_LIST>
          <OUTPUT_STMNT>(DISPLAY)
            <PRIMARY>("equal")
      else(else)
        <STMNT_LIST>
          <OUTPUT_STMNT>(DISPLAY)
            <PRIMARY>("less")
```

## Implementation Details

### AST Architecture
- **Dynamic Node Structure**: AST nodes are dynamically allocated with parent-child relationships
- **Node Components**: Each `ASTNode` has:
  - `NodeType type`: Enum representing the node's grammatical category
  - `char* value`: Optional string value (e.g., identifier name, literal value)
  - `int token_type`: Original token type for terminal nodes
  - `ASTNode** children`: Dynamic array of child node pointers
  - `int child_count`: Current number of children
  - `int child_capacity`: Allocated capacity for children array

### Key Functions
- **`createNode(NodeType, value, token_type)`**: Allocates and initializes a new AST node
- **`addChild(parent, child)`**: Adds a child node to parent's children array (auto-resizes with realloc)
- **`printAST(node, indent)`**: Recursively traverses and displays the tree with indentation
- **`nodeTypeToString(NodeType)`**: Converts node type enum to EBNF notation string
- **`freeAST(node)`**: Recursively frees all nodes and their allocated memory

### Parser Integration
All parsing functions (`parseProgram()`, `parseStatementList()`, `parseExpr()`, etc.) return `ASTNode*`:
- Each parse function creates an appropriate node type
- Child nodes are added via `addChild()` as parsing progresses
- The recursive descent parser naturally builds the tree structure
- Error recovery creates partial trees, allowing visualization of parsed portions

### Memory Management
- **Allocation**: `malloc()` for nodes, `strdup()` for string values, `realloc()` for child arrays
- **Deallocation**: `freeAST()` performs post-order traversal to free all nodes
- **Safety**: All allocations are checked; tree is freed after printing in `main()`

## Design Evolution

### Initial Implementation (Generic Nodes)
The first version used generic node types:
- `NODE_PROGRAM`, `NODE_STATEMENT_LIST`
- `NODE_DECLARATION`, `NODE_ASSIGNMENT`
- `NODE_BINARY_OP`, `NODE_UNARY_OP` (generic for all operators)

**Problem**: Didn't reflect the formal grammar structure, losing linguistic information.

### EBNF-Aligned Refactor (Current Implementation)
Updated to match the formal grammar from PPL Project Proposal Group 6.md:

**Key Changes**:
1. **Node Naming**: `NODE_PROGRAM` → `NODE_LEXC_PROGRAM`, `NODE_STATEMENT_LIST` → `NODE_STMNT_LIST`
2. **Specific Statement Types**: Separate nodes for each statement category (`NODE_DECLARATION_STMNT`, `NODE_ASSMNT_STMNT`, `NODE_INPUT_STMNT`, `NODE_OUTPUT_STMNT`)
3. **Expression Hierarchy**: Replaced generic `NODE_BINARY_OP` with specific precedence levels:
   - `NODE_LOGICAL_OP` (or/and)
   - `NODE_COMPARE_OP` (relational operators)
   - `NODE_ADDITIVE_OP` (+/-)
   - `NODE_MULTIPLICATIVE_OP` (*/÷/%)
   - `NODE_POWER` (^)
   - `NODE_UNARY_EXPR` (prefix operators)
   - `NODE_POSTFIX_EXPR` (postfix operators)
4. **Terminal Notation**: Identifiers display as `id(name)`, literals as `<PRIMARY>(value)`
5. **EBNF Output Format**: `nodeTypeToString()` returns angle-bracket notation for non-terminals

**Rationale**:
- **Traceability**: Each node maps directly to a grammar production rule
- **Pedagogical Value**: Students can see grammar rules in action
- **Debugging**: Easier to identify which production caused an error
- **Formalism**: Matches academic notation in language specification documents

### NodeType Enum Evolution
Expanded from **18 types** (v1) to **46 types** (v2) to capture full grammar granularity:
- Added all statement subtypes
- Added complete expression precedence hierarchy  
- Added component nodes (DATA_TYPE, DECL_LIST, DECL_ITEM, ELIF_CLAUSE, ELSE_CLAUSE)
- Added specific operator categories

## Testing

Successfully tested with multiple .lexc files:
- **test.lexc**: Basic declarations, assignments, I/O
- **test3.lexc**: Complex expressions, nested conditionals, repeat loops
- **test5.lexc**: Multiple elif branches, declaration lists

All tests produce correctly structured trees matching the EBNF grammar specification.

## Future Enhancements

Potential improvements for the AST system:
- [ ] Add line/column information to nodes for better error reporting
- [ ] Implement tree transformation passes (optimization, type checking)
- [ ] Add AST serialization (JSON/XML output)
- [ ] Implement visitor pattern for tree traversal
- [ ] Add semantic analysis annotations to nodes
