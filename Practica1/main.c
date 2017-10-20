#include <stdio.h>
#include "InputSystem.h"

int main() {
        initInputSystem("ex.txt");
    char currentChar = nextCharacter();
    while (currentChar != EOF){
        printf(" %c ", currentChar);
        currentChar = nextCharacter();
    }


    printf("Hello, World!\n");
    return 0;
}