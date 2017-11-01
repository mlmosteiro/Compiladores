#include <stdio.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "InputSystem.h"
#include "SintacticAnalyzer.h"
#include "ErrorManager.h"

int main(int argc, char *argv[]) {
    if(argc<2){
        showError(MISSING_ARGUMENTS,0);//TODO: MIRAR SI HACE FALTA PASAR EL .H DE DEFINICIONES
    }
    initSymbolsTable();// todo: SI SE PASA OTRO ARCHIVO, CUIDAR AQUI

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
    char* filepath = argv[1];

    printf("\n---PRUEBAS DEL MÓDULO Analizador sintactico---\n");
    initSintacticAnalyzer(filepath);
    startSintacticAnalisis();
    destroySintacticAnalyzer();

    printSymbolsTable();
    destroySymbolsTable();

    return 0;
}