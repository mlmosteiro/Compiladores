#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h" 

abb symbolsTable;

void initSymbolsTable() {
    crea(&symbolsTable);
}

symbolInput * insert(char *name, int type) {
    // Reservamos los recursos necesarios para una nueva entrada y la insertamos en la tabla.
    symbolInput* newSymbol =  (symbolInput*) malloc(sizeof(symbolInput));
    newSymbol->name = (char*) malloc(strlen(name)+1);
    strcpy(newSymbol->name,name);
    newSymbol->type = type;
    newSymbol->value.var = 0; //  pone valor a 0 incluso si es fctn.
    inserta(&symbolsTable, *newSymbol);

    return newSymbol;
}


symbolInput* search(char *name) {
    symbolInput* result;
    result = NULL;
    buscaNodo(symbolsTable, name, result);
    return result;
}


void modify() {
    //No es motivo de esta practica modificar / añadir información a la tabla de símbolos.
}

void destroySymbolsTable() {
    destruye(&symbolsTable);
    symbolsTable = NULL;
}

/* Funcion auxiliar que nos permite recorrer el árbol de forma recursiva
 * para imprimir la tabla de simbolos
 * */
void printSymbolTableRec(abb symbolsTable) {
    symbolInput E;
    if (! esVacio(symbolsTable)) {
        printSymbolTableRec(izq(symbolsTable));
        info(symbolsTable, &E);
        printf("%s - %d \n", E.name, E.type);
        printSymbolTableRec(der(symbolsTable));
    }
}

void printSymbolsTable() {
    printf("\n\n--- Tabla de símbolos ---\n");
    printSymbolTableRec(symbolsTable);
}

void searchLexicalCompAndPrintSymbolsTable(int componenteLexico){
    printf("\n\n--- Tabla de símbolos: %d ---\n", componenteLexico);
    printf("---------aqui se imprimiran las cosas----------\n");
}


