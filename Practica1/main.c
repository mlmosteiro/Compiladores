#include <stdio.h>
#include "SymbolsTable/SymbolsTable.h"
#include "SintacticAnalyzer.h"
#include "ErrorManager.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        showError(MISSING_ARGUMENTS, 0);//TODO: MIRAR SI HACE FALTA PASAR EL .H DE DEFINICIONES
    }
    char *filepath = argv[1];

    initSymbolsTable();// todo: SI SE PASA OTRO ARCHIVO, CUIDAR AQUI
    initSintacticAnalyzer(filepath);

    printf("\n---START : Análisis sintáctico---\n");
    startSintacticAnalisis();
    printf("\n---END  : Análisis sintáctico---\n");

    destroySintacticAnalyzer();

    printSymbolsTable();
    destroySymbolsTable();

    return 0;
}