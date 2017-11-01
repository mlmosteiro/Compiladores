CC= gcc -Wall -Wextra -pedantic-errors
LIBS = 

MAIN= practica1

SRCS = main.c InputSystem.c LexicalAnalyzer.c SintacticAnalyzer.c SymbolsTable/SymbolsTable.c ErrorManager.c SymbolsTable/BST.c

DEPS = Definitions.h InputSystem.h LexicalAnalyzer.h SintacticAnalyzer.h SymbolsTable/SymbolsTable.h ErrorManager.h SymbolsTable/BST.h

OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS) 
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)
 
%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(INCLUDES)

cleanall: clean 
	rm -f $(MAIN)

clean: 
	rm -f *.o *~ *.o
