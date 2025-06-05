# LOGIC SOLVER
This is a logic expression solver.

## LOGIC LAWS (TODO)
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

## SYNTAX
- and/et: `&`
- or/ou: `|`
- then/alors: `>`
- bidirectional then / bidirectionnel alors: `~`

## TESTS
### FAILING EXPRESSION
- `( ( p > q ) & ( q > r ) ) > ( p > r )`
- `( p & ( p > q ) ) > q`
- `( p > ( q & r ) ) > ( ( p > q ) & ( p > r ) )`
- `p > ( q ~ p )`
- `( ( p | q ) > r ) > ( ( p > r ) & ( q > r ) )`
- `( ( p > r ) & ( q > r ) & ( p | q ) ) > r`
- `( ( ! p & q ) | p ) ~ ( ( p | q ) & ! p ) ~ p`
### WORKING EXPRESSIONS
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
