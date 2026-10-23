.RECIPEPREFIX = >
CC      = gcc
FLEX    = flex
BISON   = bison
TARGET  = compiler
SRC     = src/parser.tab.c src/lex.yy.c src/main.c src/codegen.c

all: $(TARGET)

src/parser.tab.c src/parser.tab.h: src/parser.y
>$(BISON) -d -o src/parser.tab.c src/parser.y

src/lex.yy.c: src/lexer.l src/parser.tab.h
>$(FLEX) -o src/lex.yy.c src/lexer.l

$(TARGET): $(SRC)
>$(CC) $(SRC) -o $(TARGET)

run: $(TARGET)
>./$(TARGET) tests/correct03.c -tokens -symtab -3ac -asm

clean:
>rm -f src/parser.tab.c src/parser.tab.h src/lex.yy.c $(TARGET)
