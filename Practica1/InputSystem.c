#include <stdio.h>
#include <stdlib.h>
#include "InputSystem.h"
#include "ErrorManager.h"

/*
 * Sistema de entrada. Implementación del método centinela.
 */

#define FIRSTBLOCK 0
#define LASTBLOCK 1

FILE *file;
char firstBlock[SIZE_BLOCK + 1];
char lastBlock[SIZE_BLOCK + 1];
char *firstBlockEOF;
char *lastBlockEOF;
char *start;
char *current;

/* Función con la que se carga el bloque idblock con el contenido del
 * fichero de código fuente
 */
int loadBlock(int idBlock) {
    int succes; // variable que nos indica el numero de elementos leidos

    /* Se carga el bloque indicado y en caso de que el número de elementos
     * leidos sea menor que el tamaño del bloque, es que ya hemos leido
     * tod0 el fichero e introducimos el caracter EOF
     */

    switch (idBlock) {
        case FIRSTBLOCK:
            succes = (int) fread(firstBlock, 1, sizeof(char) * SIZE_BLOCK, file);
            if (succes < SIZE_BLOCK) {
                firstBlock[succes] = EOF;
            }
            return 1;
        case LASTBLOCK:
            succes = (int) fread(lastBlock, 1, sizeof(char) * SIZE_BLOCK, file);
            if (succes < SIZE_BLOCK) {
                lastBlock[succes] = EOF;
            }
            return 1;
        default:
            return 0;
    }
}

/*
 * Al inicio tanto el puntero de inicio como el delantero apuntan al primer caracter de un lexema.
 */
void initInputSystem(char *fileName) {
    file = fopen(fileName, "r");
    if (file == NULL) {
        showError(NOT_FILE_FOUNDED, 0);
        return;
    }

    firstBlockEOF = &firstBlock[SIZE_BLOCK];
    *firstBlockEOF = EOF;

    lastBlockEOF = &lastBlock[SIZE_BLOCK];
    *lastBlockEOF = EOF;

    start = &firstBlock[0];
    current = &firstBlock[0];

    loadBlock(FIRSTBLOCK);
}

void destroyInputSystem() {
    fclose(file); // Cerramos el archivo de código fuente
}

char nextCharacter() {
    /* Segun el patron productor consumidor, el analizador léxico le pide al sistema de entrada el siguiente componente lexico
     * En este punto es en el que verificaremos si estamos leyendo un EOF. En caso afirmativo, se verifica si se trata del final
     * de un bloque o es el final del fichero.
     *
     * En caso de final de bloque, se carga un nuevo bloque.
    */
    if (*current == EOF) {
        if (current == firstBlockEOF) {
            loadBlock(LASTBLOCK);
            current = &lastBlock[0];
        } else if (current == lastBlockEOF) {
            loadBlock(FIRSTBLOCK);
            current = &firstBlock[0];
        } else {
            return EOF;
        }
    }
    char character = *current;
    current ++;
    return character;
}

void prepareForNextLex() {
    // Situamos el puntero de inicio en la misma posicion que el puntero actual
    // para marcar el inicio de un nuevo lexema
    start = current;
}

char previousCharacter() {
    // En caso de que se retroceda estando en el incio de alguno de los bloques,
    // evitamos los EOF centinela.
    if (current == firstBlock || current == lastBlock) {
        current --;
    }
    current --;

    return *current;
}
