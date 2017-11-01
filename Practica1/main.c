#include <stdio.h>
#include "SymbolsTable/SymbolsTable.h"
#include "SintacticAnalyzer.h"
#include "ErrorManager.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        showError(MISSING_ARGUMENTS, 0);
        return MISSING_ARGUMENTS;
    }
    char *filepath = argv[1];
    char *keywords = argv[2];
    initSymbolsTable(keywords);
    initSintacticAnalyzer(filepath);

    printf("\n---START : Análisis sintáctico---\n");
    startSintacticAnalisis();
    printf("\n---END  : Análisis sintáctico---\n");

    destroySintacticAnalyzer();

    printSymbolsTable();
    destroySymbolsTable();

    return 0;
}