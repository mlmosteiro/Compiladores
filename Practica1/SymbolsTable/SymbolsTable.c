#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SymbolsTable.h"
#include "BST.h"
#include "../ErrorManager.h"

#define KEYWORDS_PATH "../Definitions.h"
#define MAX_KEYWORD_SIZE 1024
#define DELIMITER_KEYWORDS "//KEYWORDS\n"

abb symbolsTable;

void importLenguageKeyWords() {
    FILE *file;
    file = fopen(KEYWORDS_PATH, "r");
    char *line = (char *) malloc(sizeof(char) * MAX_KEYWORD_SIZE);
    int i = 0;
    if (file == NULL) {
        showError(NOT_KEYWORDS_FILE_FOUNDED,0);
        return;
    }
    int beginKeyWords = 0; // Flag que nos indica si se trata del bloque de palabras reservadas

    //Read the file
    while (1) {
        if (fgets(line, MAX_KEYWORD_SIZE, file) == NULL) break;
        i ++;
        //Check if we are in the part of the keywords
        if (strcmp(DELIMITER_KEYWORDS, line) == 0) {
            if (beginKeyWords == 1) {
                break;
            } else {
                beginKeyWords = 1;
                continue;
            }
        }

        //Parsing the keywords
        if (beginKeyWords) {
            char *lexema;
            char *numberChar;
            int lexicalComponent;
            strtok(line, " ");
            lexema = strtok(NULL, " ");
            numberChar = strtok(NULL, " ");
            for (i = 0; lexema[i]; i ++) {
                lexema[i] = (char) tolower(lexema[i]);
            }
            lexicalComponent = atoi(numberChar);
            insert(lexema, lexicalComponent, - 1);
        }
    }
    free(line);
    fclose(file);
}

void initSymbolsTable() {
    crea(&symbolsTable);
    importLenguageKeyWords();
}

int sizeLexema(char* lexema){
    int i= 0;
    while(lexema[i]!='\0'){
        i++;
    }
    return i;
}

void insert(char *lexema, int componenteLexico, int numLinea) {
    symbolImput newSymbol;
    newSymbol.lexema = (char *) malloc(sizeof(char)*sizeLexema(lexema));
    strcpy(newSymbol.lexema, lexema);
    newSymbol.componenteLexico = componenteLexico;
    newSymbol.numLinea = numLinea;
    inserta(&symbolsTable, newSymbol);
}


symbolImput search(char *lexema) {
    symbolImput result;
    result.lexema= NULL;
    buscaNodo(symbolsTable, lexema, &result);
    return result;
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
    printf("\n\n--- Tabla de símbolos ---\n");
    printSymbolTableRec(symbolsTable);
}
