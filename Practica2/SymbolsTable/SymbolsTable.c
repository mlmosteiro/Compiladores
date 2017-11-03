#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SymbolsTable.h"
#include "BST.h"
#include "../ErrorManager.h"

// Tamaño máximo de una linea. En este caso coincide con el tamaño máximo del un bloque del sistema de entrada
#define MAX_LINE_SIZE 512
#define DELIMITER_KEYWORDS "//KEYWORDS\n"

abb symbolsTable;

void importLenguageKeyWords(char* keywords) {
    int i = 0;
    int beginKeyWords = 0; // Flag que nos indica si se trata del bloque de palabras reservadas
    FILE *file;
    char *line = (char *) malloc(sizeof(char) * MAX_LINE_SIZE);

    file = fopen(keywords, "r");
    if (file == NULL) {
        showError(NOT_KEYWORDS_FILE_FOUNDED, 0);
        return;
    }

    while (1) {
        if (fgets(line, MAX_LINE_SIZE, file) == NULL) break; //Leemos el archivo linea a linea

        //Si hemos llegado ya al inicio del bloque de palabras reservadas, activamos el flag
        if (strcmp(DELIMITER_KEYWORDS, line) == 0) {
            if (beginKeyWords == 1) {
                break;
            } else {
                beginKeyWords = 1;
                continue;
            }
        }

        // Parseamos las palabras reservadas
        if (beginKeyWords) {
            char *lexema;
            char *numberChar;
            int lexicalComponent;

            //Separamos la linea en componentes teniendo en cuenta ' ' como separador.
            strtok(line, " ");
            lexema = strtok(NULL, " ");
            numberChar = strtok(NULL, " ");

            // cambiamos al formato adecuado: Minusculas y numero entero
            for (i = 0; lexema[i]; i ++) {
                lexema[i] = (char) tolower(lexema[i]);
            }
            lexicalComponent = atoi(numberChar);

            // Insertamos la palabra reservada en la tabla de simbolos.
            insert(lexema, lexicalComponent, - 1);
        }
    }
    free(line);
    fclose(file);
}

void initSymbolsTable(char* keywords) {
    crea(&symbolsTable);
    importLenguageKeyWords(keywords);
}

/* Función que devuelve el número de caracteres que conforman un lexema
*/
int sizeLexema(char *lexema) {
    int i = 0;
    while (lexema[i] != '\0') {
        i ++;
    }
    return i;
}

void insert(char *lexema, int componenteLexico, int numLinea) {
    // Reservamos los recursos necesarios para una nueva entrada y la insertamos en la tabla.
    symbolImput newSymbol;
    newSymbol.lexema = (char *) malloc(sizeof(char) * sizeLexema(lexema));
    strcpy(newSymbol.lexema, lexema);
    newSymbol.componenteLexico = componenteLexico;
    newSymbol.numLinea = numLinea;
    inserta(&symbolsTable, newSymbol);
}

symbolImput search(char *lexema) {
    symbolImput result;
    result.lexema = NULL;
    buscaNodo(symbolsTable, lexema, &result);
    return result;
}

void modify() {
    //No es motivo de esta practica modificar / añadir información a la tabla de símbolos.
}

void destroySymbolsTable() {
    destruye(&symbolsTable);
}

/* Funcion auxiliar que nos permite recorrer el árbol de forma recursiva
 * para imprimir la tabla de simbolos
 * */
void printSymbolTableRec(abb symbolsTable) {
    symbolImput E;
    if (! esVacio(symbolsTable)) {
        printSymbolTableRec(izq(symbolsTable));
        info(symbolsTable, &E);
        printf("%s - %d\n", E.lexema, E.componenteLexico);
        printSymbolTableRec(der(symbolsTable));
    }
}

void printSymbolsTable() {
    printf("\n\n--- Tabla de símbolos ---\n");
    printSymbolTableRec(symbolsTable);
}
