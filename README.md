# LOGIC SOLVER
This is a logic expression solver.

## TODO:
- [ ] [Operator Precedence](http://logic.stanford.edu/intrologic/dictionary/operator_precedence.html)

# THEORY
## POWER SET - SIMPLIFICATION
The expression `(p ∧ q) ∨ (r ∧ (p ∨ q))` is at it's simplest.

n | $n \in{N}$ | n > 1  
Let U be a set of n.

Let F(A) be:
Arranged such that:  
$(A_{0,0} ∧ A_{0,1} ∧ ... ∧ A_{0,n-1}) v ... v (A_{n,0} ∧ A_{n,1} ∧ ... ∧ A_{n,n-1})$  

OR (equivalent):
$(A_{0,0} v A_{0,1} v ... v A_{0,n-1}) ∧ ... ∧ (A_{n,0} v A_{n,1} v ... v A_{n,n-1})$  

### FULL POWER SET
In a logical expression where:  
Let A = $2^{U}$  

F(A) would be a TAUTOLOGY, because all possiblities are accounted for.  
In addition, it's not possible to have a CONTRADICTION.  

### PARTIAL POWER SET
In a logical expression where:  
k | $k \in{N}$ | k < n && n > 2  
Let A = $\binom{U}{k}$  

F(A) would not be TAUTOLOGY, because all possiblities are accounted for.  
But, it still has the property of not having a CONTRADICTION.  

## LOGIC LAWS
- [x] Idempotent Law
- [x] Complementary Law
- [x] Absorption Law
- [x] Associative Law
- [x] Commutative Law
- [x] Dommination Law
- [x] Identity Law
- [x] Involution Law (`! ! x` is equivalent to `x`)
- [ ] Distribution Law
- [ ] DeMorgan Law

# SYNTAX
- and/et: `&`
- or/ou: `|`
- then/alors: `>`
- bidirectional then / bidirectionnel alors: `~`

# TESTS
## FAILING EXPRESSION
- `( p & p ) & ( p | p )`
- `( ( p > q ) & ( q > r ) ) > ( p > r )`
- `( p & ( p > q ) ) > q`
- `( p > ( q & r ) ) > ( ( p > q ) & ( p > r ) )`
## WORKING EXPRESSIONS
- `p & p` ≡ `p`
- `( p & ( q | r ) ) | p` ≡ `p`
- `( p & q | r ) | p` ≡ `p`
- `( ( p | q ) & p ) > p` ≡ `T` 
- `( ( p & q ) | p ) ~ ( ( p | q ) & p ) ~ p` ≡ `p`
- `p > ( p > p )` ≡ `T`
