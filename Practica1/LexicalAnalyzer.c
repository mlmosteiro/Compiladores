#include <stdlib.h>
#include <stdio.h>
#include "LexicalAnalyzer.h"
#include "InputSystem.h"
#include "SymbolsTable/SymbolsTable.h"

char* lexemaBuffer;
int mult = 1;

void initLexicalAnalyzer() {
    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

void extendLexemaBuffer() {
    lexemaBuffer = (char *) realloc(lexemaBuffer, ((mult++) * sizeof(char) * SIZE_SA));
}

void restartLexemaBuffer() {
    free(lexemaBuffer);
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

char * nextLexicalComponent() {
    char character=' ';
    int indexOfChar = -1;

    restartLexemaBuffer();


    while (character != '\000') {
        //Simulamos que encontramos un lexema
        for(int i = 0; i<6;i++){
            character = nextCharacter();
            if(character == '\000' && indexOfChar == 0){
                return NULL;
            }

            indexOfChar++;
            printf("SA: %c\n", character);
            if(indexOfChar == SIZE_SA){
                extendLexemaBuffer();
            }
            lexemaBuffer[indexOfChar]=character;
        }

        insert(lexemaBuffer, indexOfChar);
        return lexemaBuffer;
    }
}


void destroyLexicalAnalyzer(){
    free(lexemaBuffer);
}
