# Makefile

CC      = gcc
CFLAGS  = -Wall -g -DENABLE_LLVM=0
LEX     = flex
YACC    = bison
YFLAGS  = -d --defines=parser.tab.h

TARGET  = peixe
OBJS    = lex.yy.o parser.tab.o main.o llvm_codegen.o

all: $(TARGET)

parser.tab.c parser.tab.h: parser.y
	$(YACC) $(YFLAGS) parser.y

lex.yy.c: lexer.l parser.tab.h
	$(LEX) lexer.l

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

llvm_codegen.o: llvm_codegen.c llvm_codegen.h
	$(CC) $(CFLAGS) -c llvm_codegen.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lfl

clean:
	rm -f $(TARGET) *.o lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean
