#Equi CFG

Right Associative: a\*b\*c = a\*(b\*c)  
Left Associative: a\*b\*c = (a\*b)\*c  

##Operators (Highest to Lowest) and Associativity
 * (L) Function calls: function(...)
 * (L) Declaration/Assignment
 * (R) Unary Operations
 * (L) Multiplication/Division: a\*b, a/b
 * (L) Addition/Subtraction: a + b, a - b
 * (L) Comparison: a > b, a >= b, a < b, a <= b
 * (L) Equality: a == b, a != b
 * (L) Commas: a , b

##CFG
line -> expression ";";   
expression -> commas;  
commas -> equality ("," equality)\*;  
equality -> comparison (("!=" | "==") comparison)\*;  
comparison -> additive ((">" | ">=" | "<=" | "<") additive)\*;  
additive -> multiplicative (("+"|"-") multiplicative)\*;  
multipicative -> unary (("/" | "\*") unary)\*;  
unary -> ("!" | "-") unary | declaration | function | primary;  
declaration -> TOKEN declaration | (TOKEN "=")\* expression;
function -> TOKEN "(" expression ")";  
primary -> NUMBER | STRING | "false" | "true" | "null" | "(" expression ")" | TOKEN


