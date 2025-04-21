# Compilador
CC = gcc
CFLAGS = -Wall -g

# Arquivos fonte
LEX = lexer.l
YACC = parser.y

# Arquivos gerados
LEX_C = lex.yy.c
YACC_C = parser.tab.c
YACC_H = parser.tab.h

# Executável final
EXEC = meu_compilador

all: $(EXEC)

$(EXEC): $(LEX_C) $(YACC_C) main.c
	gcc -Wall -g -o $(EXEC) $(YACC_C) $(LEX_C) main.c

$(LEX_C): $(LEX) $(YACC_H)
	flex $(LEX)

$(YACC_C) $(YACC_H): $(YACC)
	bison -d -o parser.tab.c parser.y

clean:
	rm -f $(LEX_C) $(YACC_C) $(YACC_H) $(EXEC)
