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

    // En caso de que se pase el nombre de un archivo como argumento, lo analizamos primero
    if (argc == 2) {
        loadFile(argv[1]);
    }

    // Asignamos la entrada y salida por defecto de flex
    yyin = stdin;
    yyout = stdout;
   

    printf("\n-----------------------------------------------------------\n");
    printf("---------------  Consola matemática - V1.0  ---------------\n");
    printf("-----------------------------------------------------------\n");
    printf("----------------------  AYUDA --> :?  ---------------------\n");
    printf("-----------------------------------------------------------\n");

    yyparse();
    destroySymbolsTable();

    return 0;
}