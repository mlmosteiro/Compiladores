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
    symbolInput newSymbol;
    newSymbol.name = (char*) malloc(strlen(name)+1);
    strcpy(newSymbol.name,name);
    newSymbol.type = type;
    newSymbol.value.var = 0.0; //  pone valor a 0 incluso si es fctn.
    return inserta(&symbolsTable, newSymbol);
}


symbolInput* search(char *name) {
    symbolInput* result;
    result = NULL;
    result = buscaNodo(symbolsTable, name);
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
    printSymbolTableRec(symbolsTable);
}

/* Funcion auxiliar que nos permite recorrer el árbol de forma recursiva
 * para imprimir la tabla de simbolos segun el tipo del elemento que buscamos
 * */
void printSymbolByTypeRec(abb symbolsTable, int type){
    symbolInput E;
    if (! esVacio(symbolsTable)) {
        printSymbolByTypeRec(izq(symbolsTable),type);
        info(symbolsTable, &E);
        if(E.type == type) {
            printf("%s - %d \n", E.name, E.type);
        }
        printSymbolByTypeRec(der(symbolsTable), type);
    }
}
void printSymbolByType(int type){
    printSymbolByTypeRec(symbolsTable,type);
}


