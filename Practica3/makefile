CC= gcc -pedantic-errors
LIBS = -lfl -lm

MAIN= ConsolaMatematicaV1.0

SRCS = main.c  SymbolsTable/SymbolsTable.c ErrorManager.c SymbolsTable/BST.c  consolaMatematica.tab.c lex.yy.c Definitions.c

DEPS =  SymbolsTable/SymbolsTable.h ErrorManager.h SymbolsTable/BST.h consolaMatematica.tab.h lex.yy.h Definitions.h

OBJS = $(SRCS:.c=.o)


$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(INCLUDES)

cleanall: clean 
	rm -f $(MAIN)

clean: 
	find . -name '*.o' -exec rm -r {} \;
