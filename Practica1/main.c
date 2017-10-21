#include <stdio.h>
#include "InputSystem.h"

int main() {
    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    char currentChar = nextCharacter();
    while (currentChar != '\000'){
        printf(" %c ", currentChar);
        currentChar = nextCharacter();
    }


    endInputSystem();

    printf("Hello, World!\n");
    return 0;
}