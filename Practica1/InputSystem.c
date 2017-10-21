#include <stdio.h>
#include "InputSystem.h"

/*
 * Sistema de entrada. Implementación del método centinela.
 */

#define FIRSTBLOCK 0
#define LASTBLOCK 1

FILE *file;
char firstBlock [SIZE_BLOCK+1];
char lastBlock [SIZE_BLOCK+1];
char *firstBlockEOF;
char *lastBlockEOF;
char *start;
char *current;

int numberOfBytesRead;

//  Leemos un bloque nuevo
int loadBlock(int idBlock) {
    switch (idBlock) {
        case FIRSTBLOCK:
            fread(firstBlock, 1, SIZE_BLOCK, file);
            return 1;
        case LASTBLOCK:
            fread(lastBlock, 1, SIZE_BLOCK, file);
            return 1;
        default:
            return 0;
    }
}

/*
 * Al inicio tanto el puntero de inicio como el delantero apuntan al primer caracter de un lexema.
 */
int initInputSystem(char *fileName){
    file = fopen(fileName, "r");
    if(file == NULL){
        return -1; //TODO: TRATAMIENTO DE ERRORES!!
    }

    firstBlockEOF = &firstBlock[SIZE_BLOCK];
    *firstBlockEOF = '\000';

    lastBlockEOF = &lastBlock[SIZE_BLOCK];
    *lastBlockEOF = '\000';

    start = &firstBlock[0];
    current = &firstBlock[0];

    loadBlock(FIRSTBLOCK);
    return 0;
}


// Cerramos el archivo de código fuente
int endInputSystem(){

    fclose(file);
}

/*
 * Segun el patron productor consumidor, el analizador léxico le pide al sistema de entrada el siguiente componente lexico
 * En este punto es en el que verificaremos si estamos leyendo un EOF. En caso afirmativo, se verifica si se trata del final
 * de un bloque o es el final del fichero.
 *
 * En caso de final de bloque, se carga un nuevo bloque.
*/

char nextCharacter(){

    if(*current =='\000') {
        if(current == firstBlockEOF) {
            loadBlock(LASTBLOCK);
            current = &lastBlock[0];
        }
        else if (current  == lastBlockEOF) {
            loadBlock(FIRSTBLOCK);
            current = &firstBlock[0];
        }
        else {
            return '\000';
        }

    }
    char character = *current;
    *current++;
    return character;
}


void previousChar() {
    *current--;
}
