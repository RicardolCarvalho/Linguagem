# Linguagem

PROGRAM     = { STATEMENT } ;

STATEMENT   = ( VAR_DECL | ASSIGNMENT | PRINT | IF | WHILE ), ";" ;

VAR_DECL    = "let", IDENTIFIER, ":", TYPE, "=", EXPRESSION ;

ASSIGNMENT  = IDENTIFIER, "=", EXPRESSION ;

PRINT       = "print", "(", EXPRESSION, ")" ;

IF          = "if", "(", EXPRESSION, ")", BLOCK, [ "else", BLOCK ] ;

WHILE       = "while", "(", EXPRESSION, ")", BLOCK ;

BLOCK       = "{", { STATEMENT }, "}" ;

EXPRESSION  = LOGIC_EXPR ;

LOGIC_EXPR  = COMP_EXPR, { ("&&" | "||"), COMP_EXPR } ;

COMP_EXPR   = CONCAT_EXPR, { ("==" | "!=" | "<" | ">" | "<=" | ">="), CONCAT_EXPR } ;

CONCAT_EXPR = TERM, { "++", TERM } ;

TERM        = FACTOR, { ("+" | "-"), FACTOR } ;

FACTOR      = UNARY, { ("*" | "/"), UNARY } ;

UNARY       = ( ("-" | "!"), UNARY ) | PRIMARY ;

PRIMARY     = NUMBER | BOOLEAN | STRING | IDENTIFIER | "(", EXPRESSION, ")" ;

TYPE        = "i32" | "bool" | "str" ;

BOOLEAN     = "true" | "false" ;

STRING      = '"', { CHAR }, '"' ;

IDENTIFIER  = LETTER, { LETTER | DIGIT | "_" } ;

NUMBER      = DIGIT, { DIGIT } ;

CHAR        = any printable character except '"' ;

LETTER      = ( a | ... | z | A | ... | Z ) ;

DIGIT       = ( 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ) ;
