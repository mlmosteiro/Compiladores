#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolsTable.h"
#include "BST.h"

abb symbolsTable;

void importLenguageKeyWords() {
    //todo: añadi las palabras clave del lenguaje

    /*
    //Parsing the keywords

    if(beginKeyWords){
        char *lexema;
        char *numberChar;
        int lexicalComponent;
        strtok (line," ");
        lexema = strtok(NULL," ");
        numberChar = strtok(NULL," ");
        for(int i = 0; lexema[i]; i++)
            lexema[i] = tolower(lexema[i]);

        lexicalComponent = atoi(numberChar);
        //printf("El resultado lexema = %s, lexicalComponent = %d\n",lexema,lexicalComponent);
        strcpy(item->lexema,lexema);
        item->lexicalComponent = lexicalComponent;
        insertItem(*item);
        numberOfKeyWords++;
    }*/
}

void initSymbolsTable() {
    crea(&symbolsTable);
    importLenguageKeyWords();
}

void insert(char *lexema, int componenteLexico) {
    symbolImput *newSymbol;
    newSymbol = (symbolImput *) malloc(sizeof(symbolImput));
    newSymbol->lexema = (char *) malloc(sizeof(lexema));
    strcpy(newSymbol->lexema, lexema);
    newSymbol->componenteLexico = componenteLexico;
    inserta(&symbolsTable, *newSymbol);
}

int search(char *lexema) {
    symbolImput *result;
    buscaNodo(symbolsTable, lexema, result);
    return result == NULL ? - 1 : result->componenteLexico;
}

void modify() {
    //No es motivo de esta practica modificar / añadir información a la tabla de símbolos.
}

void destroySymbolsTable() {
    destruye(&symbolsTable);
}

void printSymbolTableRec(abb symbolsTable) {
    symbolImput E;
    if (! esVacio(symbolsTable)) {
        info(symbolsTable, &E);
        printf("%s - %d\n", E.lexema, E.componenteLexico);
        printSymbolTableRec(izq(symbolsTable));
        printSymbolTableRec(der(symbolsTable));
    }
}

void printSymbolsTable() {
    printf("Tabla de símbolos:\n");
    printSymbolTableRec(symbolsTable);
}

