# LOGIC SOLVER
This is a logic expression solver.

# THEORY
## POWER SET - SIMPLIFICATION
The expression `(p ∧ q) ∨ (r ∧ (p ∨ q))` is at it's simplest.

n | n \in{N} | n > 1  
Let U be a set of n.

Let F(A) be:
Arranged such that:  
(A_{0,0} ∧ A_{0,1} ∧ ... ∧ A_{0,n-1}) v ... v (A_{n,0} ∧ A_{n,1} ∧ ... ∧ A_{n,n-1})  

OR (equivalent):
(A_{0,0} v A_{0,1} v ... v A_{0,n-1}) ∧ ... ∧ (A_{n,0} v A_{n,1} v ... v A_{n,n-1})  

### FULL POWER SET
In a logical expression where:  
Let A = $$2^{U}$$  

F(A) would be a TAUTOLOGY, because all possiblities are accounted for.  
In addition, it's not possible to have a CONTRADICTION.  

### PARTIAL POWER SET
In a logical expression where:  
k | k \in{N} | k < n && n > 2  
Let A = $$\binom{U}{k}$$  

F(A) would not be TAUTOLOGY, because all possiblities are accounted for.  
But, it still has the property of not having a CONTRADICTION.  




## TEST DATA
```
1. `(A ^ B | C) > (D ^ E) ~ (A | B) ^ (C > D)`
2. `((A ^ B) > (C | D)) ^ (E > (A ~ B ^ C))`
3. `((A | B) ^ C) > ((D ^ E) ~ (A | C))`
4. `(A ^ (B | C)) > ((D ^ E) ~ (B > A))`
5. `((A | B) ^ C) > ((D ^ E) ~ (C | B))`
6. `(A ^ (B > C)) ~ (D ^ E | (A > B))`
7. `((A ^ B) > (C | D)) ^ (E > (A ~ C))`
8. `(A ^ (B | C)) > ((D ~ E) ^ (A > C))`
9. `(A | B) ^ (C > D) ~ (E ^ (A | C))`
10. `(A ^ (B | C)) > ((D ~ E) ^ (A > C))`
11. `((A ^ B) | (C > D)) ~ (E ^ (A > C))`
12. `(A ^ B) > ((C | D) ^ E) ~ (A | (C > D))`
13. `(A ^ (B | C)) > ((D ^ E) ~ (A | B))`
14. `((A ^ B) | C) > (D ^ (E ~ A))`
15. `(A ^ B) > ((C | D) ~ (A ^ E))`
16. `(A | (B ^ C)) ~ ((D ^ E) > (A | C))`
17. `((A ^ B) ~ (C > D)) | (E ^ (A > C))`
18. `(A | B) > ((C ^ D) ~ (E ^ (A ^ C)))`
19. `(A ^ B | C) > ((D ^ E) ~ (C > A))`
20. `(A ^ (B | C)) ~ (D ^ E | (A > B))`
21. `((A ^ B) > (C | D)) ~ (E ^ (A > C))`
22. `((A | B) ^ (C > D)) > (E ^ (A > B))`
23. `((A ^ B) | (C ^ D)) > (E ~ (A > C))`
24. `((A ^ (B | C)) > D) ^ (E ~ (A > B))`
25. `(A ^ B) > ((C ^ D) ~ (A | C))`
26. `(A ^ (B | C)) > (D ^ (E ~ (A ^ C)))`
27. `((A ^ B) | C) > (D ^ E) ~ (A > C)`
28. `((A ^ B) | (C > D)) > (E ^ A)`
29. `((A | B) ^ (C > D)) ~ (E ^ (A > C))`
30. `(A | B) ^ ((C ^ D) > (E ^ (A > C)))`
31. `(A ^ B) ~ (C | (D ^ E)) > (A | B)`
32. `(A ^ B) > ((C | D) ^ (E ~ A))`
33. `(A ^ (B | C)) > (D ^ E) ~ (A ^ B)`
34. `(A ^ B | C) ~ ((D ^ E) > (A > C))`
35. `((A ^ B) > (C | D)) ~ ((E ^ (A > B)) | C)`
36. `((A ^ B) | (C > D)) ~ ((E ^ A) | (C ^ D))`
37. `(A ^ (B > C)) | (D ^ (E ^ A)) > B`
38. `((A ^ B) | C) > (D ^ (E ~ A))`
39. `((A ^ B) | C) > (D ^ (E > A))`
40. `(A ^ (B | C)) > ((D ^ E) ~ (A ^ B))`
41. `(A ^ (B ^ C)) ~ (D | E) > (A ^ B)`
42. `(A ^ B) | (C > D) ~ (E ^ (A ^ C))`
43. `(A ^ (B | C)) > ((D ~ E) ^ (A > B))`
44. `((A ^ B) | C) > (D ^ E) ~ (A ^ (C > D))`
45. `((A ^ B) > (C | D)) ~ ((E ^ A) > (C ^ D))`
46. `(A | B) > ((C ^ D) ~ (E ^ (A | B)))`
47. `(A ^ (B | C)) ~ (D ^ E | (A > C))`
48. `(A ^ B) ~ ((C ^ D) > (A | E))`
49. `(A ^ B) | (C > D) > (E ^ (A ~ B))`
50. `(A ^ B | (C ^ D)) > ((E ^ A) | (C > D))`
51. `(A ^ B) > (C | (D ^ E)) > (A ^ B)`
52. `(A ^ (B ^ C)) > (D | (E > A))`
53. `(A ^ B | (C > D)) ~ (E ^ (A > C))`
54. `((A ^ B) ~ C) > (D ^ E) | (A > B)`
55. `(A | (B ^ C)) > ((D ^ E) ~ A)`
56. `(A ^ B) > ((C ^ D) | (E ~ A))`
57. `((A ^ B) | (C ^ D)) > (E ^ A)`
58. `(A ^ B) | ((C ^ D) ~ (E > A))`
59. `(A ^ B | C) > (D ^ (E ~ C))`
60. `(A ^ (B | C)) > ((D ^ E) ~ (A | B))`
61. `(A | (B ^ C)) > ((D ^ E) ~ A)`
62. `((A ^ B) > (C | D)) ~ (E ^ (A > C))`
63. `(A ^ B) > ((C ^ D) | E) ~ (A > C)`
64. `((A ^ B) | C) > ((D ^ E) ~ (A > C))`
65. `(A ^ (B | C)) ~ ((D ^ E) > (A ^ C))`
66. `(A ^ B) ~ (C | (D ^ E)) > (A ^ B)`
67. `((A ^ B) > C) ~ ((D ^ E) > A)`
68. `((A | B) ^ (C > D)) > (E ^ (A ~ B))`
69. `((A ^ B) | C) > (D ^ E) ~ (A > B)`
70. `(A ^ (B | C)) > ((D ^ E) | (A > B))`
71. `(A ^ (B ^ C)) ~ ((D ^ E) > (A ^ C))`
72. `(A ^ B) > ((C | D) ^ (E ^ A))`
73. `(A ^ B | (C ^ D)) > (E ^ (A | B))`
74. `(A ^ B) ~ (C | D) > (E ^ A)`
75. `((A ^ B) | C) ~ ((D ^ E) > (A ^ C))`
76. `(A ^ B) > ((C ^ D) ~ (E | A))`
77. `(A ^ (B ^ C)) > (D | (E ^ A))`
78. `((A | B) ^ (C > D)) > (E ^ (A | C))`
79. `((A ^ B) ~ (C > D)) | (E ^ (A | C))`
80. `(A ^ B) ~ ((C ^ D) | (E > A))`
81. `((A ^ B) | C) > ((D ^ E) ~ (A > B))`
82. `(A ^ B) ~ (C | (D ^ E)) > (A ^ C)`
83. `(A ^ B | C) > ((D ^ E) ~ (A > B))`
84. `(A ^ (B ^ C)) ~ (D | (E ^ A))`
85. `(A ^ B) > ((C ^ D) | E) ~ (A ^ C)`
86. `(A ^ B) > (C | (D ^ E)) ~ (A ^ C)`
87. `((A ^ B) | (C ^ D)) > (E ^ A)`
88. `(A ^ B) > ((C ^ D) ~ (E ^ C))`
89. `(A ^ (B | C)) > ((D ^ E) ~ (A ^ B))`
90. `((A ^ B) ~ (C > D)) | (E ^ (A | B))`
91. `(A ^ B) | (C > D) ~ ((E ^ A) > (C ^ D))`
92. `((A ^ B) | C) > (D ^ E) ~ (A ^ (C > D))`
93. `(A ^ B) > ((C ^ D) | (E ^ A))`
94. `((A ^ B) | C) > ((D ^ E) ~ (A ^ C))`
95. `(A ^ B | C) > (D ^ E) ~ (A | B)`
96. `(A ^ B) ~ ((C | D) ^ (E ^ A))`
97. `((A ^ B) | (C ^ D)) > ((E ^ A) ~ B)`
98. `(A ^ B) ~ (C ^ D) > (E ^ A)`
99. `(A ^ (B ^ C)) > ((D ^ E) ~ (A ^ C))`
100. `((A ^ B) | C) ~ ((D ^ E) > A)`
```
