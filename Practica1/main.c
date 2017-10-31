#include <stdio.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "InputSystem.h"
#include "SintacticAnalyzer.h"

int main() {
    initSymbolsTable();

    /*printf("---PRUEBAS DEL MÓDULO Sistema de Entrada---\n");

    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/concurrentSum.go");
    char currentChar = nextCharacter();
    while (currentChar != EOF){
        printf(" %c ", currentChar);
        currentChar = nextCharacter();
    }
    destroyInputSystem();
*/

 /*   printf("\n---PRUEBAS DEL MÓDULO Analizador lexico---\n");
    initLexicalAnalyzer();
    lexemaOutput componenteLexico;
    do {
        componenteLexico = nextLexicalComponent();
        printf("Componente léxico: %d\n", componenteLexico.compLex);
    }while (componenteLexico.compLex!=EOF);
    destroyLexicalAnalyzer();
*/
    printf("\n---PRUEBAS DEL MÓDULO Analizador sintactico---\n");
    initSintacticAnalyzer();
    startSintacticAnalisis();
    destroySintacticAnalyzer();

    printSymbolsTable();
    destroySymbolsTable();

    return 0;
}