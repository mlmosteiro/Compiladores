#include <stdio.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "InputSystem.h"

int main() {
    initSymbolsTable();

    printf("---PRUEBAS DEL MÓDULO Sistema de Entrada---\n");

    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    char currentChar = nextCharacter();
    while (currentChar != '\000'){
        printf(" %c ", currentChar);
        currentChar = nextCharacter();
    }
    destroyInputSystem();



    printf("\n---PRUEBAS DEL MÓDULO Analizador lexico---\n");

    initLexicalAnalyzer();
    char *componenteLexico;
    do {
        componenteLexico = nextLexicalComponent();
        printf("Componente léxico: %s\n", componenteLexico);
    }while (componenteLexico!=NULL);
    destroyLexicalAnalyzer();

    printSymbolsTable();
    destroySymbolsTable();
    printf("Hello, World!\n");
    return 0;
}