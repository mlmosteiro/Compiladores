CC= gcc -Wall -Wextra -pedantic-errors
LIBS = -lfl

MAIN= practica2

SRCS = main.c LexicalAnalyzer.c SintacticAnalyzer.c SymbolsTable/SymbolsTable.c ErrorManager.c SymbolsTable/BST.c lex.yy.c

DEPS = Definitions.h LexicalAnalyzer.h SintacticAnalyzer.h SymbolsTable/SymbolsTable.h ErrorManager.h SymbolsTable/BST.h

OBJS = $(SRCS:.c=.o)


$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)


%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(INCLUDES)

lex.yy.c: flexFile.l
	flex flexFile.l

cleanall: clean 
	rm -f $(MAIN)

clean: 
	find . -name '*.o' -exec rm -r {} \;
