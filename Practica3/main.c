#include <stdio.h>
#include "consolaMatematica.tab.h"
#include "Definitions.h"
#include "lex.yy.h"

int main(int argc, char *argv[]) {
    // Inicializamos la tabla de símbolos, a la que pueden acceder todos los otros modulos del programa
    initSymbolsTable();
    loadFunctions(functions);
    loadConstants(constants);

    
    //asigns default input and output for flex
    yyin = stdin;
    yyout = stdout;
   

    if (argc == 2) {
        char *filepath = argv[1];
        //TODO: CARGAR AQUI EL SCRIPT
    }

    printf("\nWelcome! type \":?\" an press \"Enter\" to get the system help\n");


    yyparse();
    fclose(yyin);
    destroySymbolsTable();

    return 0;
}