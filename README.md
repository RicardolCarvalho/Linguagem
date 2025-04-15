PROGRAM = { STATEMENT } ;
<br>
STATEMENT = ( ASSIGNMENT | PRINT | IF | WHILE ) , ";" ;
<br>
ASSIGNMENT = IDENTIFIER , "=" , EXPRESSION ;
<br>
PRINT = "fale" , "(" , EXPRESSION , ")" ;
<br>
IF = "se" , "(" , EXPRESSION , ")" , BLOCK , [ "senão" , BLOCK ] ;
<br>
WHILE = "enquanto" , "(" , EXPRESSION , ")" , BLOCK ;
<br>
BLOCK = "{" , { STATEMENT } , "}" ;
<br>
EXPRESSION = TERM , { ("+" | "-" | "++") , TERM } ;
<br>
TERM = FACTOR , { ("*" | "/") , FACTOR } ;
<br>
FACTOR = (("+" | "-") , FACTOR) | NUMBER | STRING | BOOL | IDENTIFIER | "(" , EXPRESSION , ")" ;
<br>
IDENTIFIER = LETTER , { LETTER | DIGIT | "_" } ;
<br>
NUMBER = DIGIT , { DIGIT } ;
<br>
STRING = '"' , { CHAR } , '"' ;
<br>
BOOL = "verdadeiro" | "falso" ;
<br>
LETTER = "a" | ... | "z" | "A" | ... | "Z" ;
<br>
DIGIT = "0" | "1" | ... | "9" ;
<br>
CHAR = qualquer caractere visível exceto aspas ;
