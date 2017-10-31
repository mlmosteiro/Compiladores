#include <stdio.h>
#include <stdlib.h>
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
    int succes;
    switch (idBlock) {
        case FIRSTBLOCK:
            succes = (int) fread(firstBlock, 1, sizeof(char) * SIZE_BLOCK, file);
            if (succes < SIZE_BLOCK){
                firstBlock[succes] = EOF;
            }
            return 1;
        case LASTBLOCK:
            succes = (int) fread(lastBlock, 1, sizeof(char) * SIZE_BLOCK, file);
            if (succes < SIZE_BLOCK){
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
int initInputSystem(char *fileName){
    file = fopen(fileName, "r");
    if(file == NULL){
        return -1; //TODO: TRATAMIENTO DE ERRORES!!
    }

    firstBlockEOF = &firstBlock[SIZE_BLOCK];
    *firstBlockEOF =  EOF;

    lastBlockEOF = &lastBlock[SIZE_BLOCK];
    *lastBlockEOF =  EOF;

    start = &firstBlock[0];
    current = &firstBlock[0];

    loadBlock(FIRSTBLOCK);
    return 0;
}


// Cerramos el archivo de código fuente
void destroyInputSystem(){
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

    if(*current == EOF) {
        if(current == firstBlockEOF) {
            loadBlock(LASTBLOCK);
            current = &lastBlock[0];
        }
        else if (current  == lastBlockEOF) {
            loadBlock(FIRSTBLOCK);
            current = &firstBlock[0];
        }
        else{
            return EOF;
        }
    }
    char character = *current;
    *current++;
    return character;
}


char previousCharacter() {
    *current--;
    return *current;
}
