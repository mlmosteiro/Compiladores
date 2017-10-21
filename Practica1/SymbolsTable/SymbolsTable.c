#include <stdlib.h>
#include "SymbolsTable.h"

abb* symbolsTable;

void importLenguageKeyWords(){
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

void initSymbolsTable(){
    initTree(symbolsTable);
    importLenguageKeyWords();

}

void insert(char* lexema, int componenteLexico){
    symbolImput *newSymbol;
    newSymbol = (symbolImput*)malloc(sizeof(symbolImput));
    newSymbol->lexema = lexema;
    newSymbol->componenteLexico=componenteLexico;
    inserta(symbolsTable, *newSymbol);
}

int search(char* lexema){
    symbolImput* result;
    buscarNodo(symbolsTable,lexema,result);
    return result == NULL ? -1 : result->componenteLexico;

}

void modify(){
    //No es motivo de esta practica modificar / añadir información a la tabla de símbolos.
}

void destroySymbolsTable(){
    destruye(symbolsTable);
}