# Equi CFG

Right Associative: a\*b\*c = a\*(b\*c)  
Left Associative: a\*b\*c = (a\*b)\*c  

## Operators (Highest to Lowest) and Associativity
 * (L) Function calls: function(...)
 * (L) Declaration/Assignment
 * (R) Unary Operations
 * (L) Multiplication/Division: a\*b, a/b
 * (L) Addition/Subtraction: a + b, a - b
 * (L) Comparison: a > b, a >= b, a < b, a <= b
 * (L) Equality: a == b, a != b
 * (L) Commas: a , b

## CFG
code -> (block | line)\*;  

block -> logicalBlock | loopBlock;  

logicalBlock -> ((("if" | ("else" "if")) "(" expression ")") | "else") (("{" code "}" (";")?) | (line | block));  

loopBlock -> doWhileLoop | whileLoop | forLoop;  
doWhileLoop -> "do" ( (line | block) | "{" code "}" ) "while" "(" expression ")" ";";  
whileLoop -> "while" "(" expression ")" ( (line | block) | "{" code "}" )  ";"?;  
forLoop -> "for" "(" expression ";" expression ";" expression ")" ( (line | block) | "{" code "}" )  ";"?;  

line -> expression ";";   


expression -> commas;  
commas -> equality ("," equality)\*;  
equality -> comparison (("!=" | "==") comparison)?;  
comparison -> additive ((">" | ">=" | "<=" | "<") additive)?;  
additive -> multiplicative (("+"|"-") additive)?;  
multiplicative -> unary (("/" | "\*" | "%") multiplicative)?;  
unary -> ("!" | "-") unary | declaration | function | primary;  
declaration -> (TOKEN | "("")") ("[" (NUMBER | TOKEN)? "]")? (TOKEN | declaration) | (TOKEN "=")+ expression;  
function -> primary "(" expression ")";  
primary -> special | array | NUMBER | STRING | "false" | "true" | "null" | "(" expression ")" | TOKEN;  
array -> TOKEN "[" (NUMBER | TOKEN) "]";  

special -> "break" | "continue";  

