#include <stdio.h>
#include "consolaMatematica.tab.h"
#include "Definitions.h"
#include "lex.yy.h"

int loadFile(char* filepath);

int main(int argc, char *argv[]) {
    // Inicializamos la tabla de símbolos, a la que pueden acceder todos los otros modulos del programa
    initSymbolsTable();
    loadFunctions(functions);
    loadConstants(constants);

    
    // Asignamos la entrada y salida por defecto de flex
    yyin = stdin;
    yyout = stdout;
   

    if (argc == 2) {
        loadFile(argv[1]);
    }

    printf("\n-----------------------------------------------------------\n");
    printf("---------------  Consola matemática - V1.0  ---------------\n");
    printf("-----------------------------------------------------------\n");
    printf("----------------------  Ayuda --> :?  ---------------------\n");
    printf("-----------------------------------------------------------\n");

    yyparse();
    destroySymbolsTable();

    return 0;
}