# Logic Expression Simplifier

This project is a C-based command-line tool that simplifies propositional logic expressions. It takes a logical expression as input, simplifies it using logical laws, and outputs the minimal form. This was developed as a university project to explore the practical application of data structures and algorithms in simplifying logical statements.

For a comprehensive overview of the project, including the underlying theory and implementation details, please see the full [project report](Report.pdf) (written in French).

## How It Works

The simplification process involves three main stages:

1.  **Lexical Analysis (Lexer)**: The input string is tokenized into a more efficient `ENUM` representation.
2.  **Parsing**: A binary expression tree is constructed from the tokenized input. This is managed by a finite state machine that correctly handles operator precedence.
3.  **Simplification (Solver)**: A greedy algorithm traverses the binary tree to apply simplification rules sequentially. It uses logical laws such as:
      * Absorption Law
      * Idempotence Law
      * Identity and Dominance Laws

The simplified tree is then converted back into a string and displayed to the user.

## Code Architecture

The program is written in C and uses standard libraries (`<stdbool.h>`, `<stdio.h>`, `<stddef.h>`, `<string.h>`). The codebase is organized into a modular structure to separate concerns:

  * **`main.c`**: Orchestrates the program flow.
  * **`parser.c`**: Handles lexical analysis and tree construction.
  * **`solver.c`**: Implements the simplification logic.
  * **`node.c`**: A utility library for managing the binary tree nodes.

The project is compiled using GCC and includes a `Makefile` for automating the build and test processes.

## Known Limitations

The current implementation has a known architectural issue that prevents the solver from reliably re-evaluating the tree after a simplification. This can lead to incorrect results for more complex, nested expressions. As a result, the tool is best suited for simpler logical expressions.

## Implementation details
### Logic Laws
- [x] Idempotent Law
- [x] Complementary Law
- [x] Absorption Law
- [x] Associative Law
- [x] Commutative Law
- [x] Dommination Law
- [x] Identity Law
- [x] Involution Law (`! ! x` is equivalent to `x`)
- [x] [Operator Precedence](http://logic.stanford.edu/intrologic/dictionary/operator_precedence.html)
- [ ] Distribution Law
- [ ] DeMorgan Law

### Syntax
- and/et: `&`
- or/ou: `|`
- then/alors: `>`
- bidirectional then / bidirectionnel alors: `~`

### Tests
#### Failing Expressions
- `( ( p > q ) & ( q > r ) ) > ( p > r )`
- `( p & ( p > q ) ) > q`
- `( p > ( q & r ) ) > ( ( p > q ) & ( p > r ) )`
- `p > ( q ~ p )`
- `( ( p | q ) > r ) > ( ( p > r ) & ( q > r ) )`
- `( ( p > r ) & ( q > r ) & ( p | q ) ) > r`
- `( ( ! p & q ) | p ) ~ ( ( p | q ) & ! p ) ~ p`
#### Working Expressions
- `p & q` ≡ `p & q`
- `p & p` ≡ `p`
- `( p & ( q | r ) ) | p` ≡ `p`
- `( p & q | r ) | p` ≡ `p`
- `( ( p | q ) & p ) > p` ≡ `T` 
- `( ( p & q ) | p ) ~ ( ( p | q ) & p ) ~ p` ≡ `p`
- `p > ( p > p )` ≡ `T`
- `( p & p ) & ( p | p )` ≡ `p`
- `! ! ! p` ≡ `! p`
- `! p` ≡ `! p`
- `! ! p` ≡ `p`
- `! ! ! ! p` ≡ `p`
- `p & ! ( p | ! q )` ≡ `F`
