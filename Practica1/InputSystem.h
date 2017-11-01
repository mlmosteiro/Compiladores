#ifndef PRACTICA1_INPUTSYSTEM_H
#define PRACTICA1_INPUTSYSTEM_H
#define SIZE_BLOCK 512  //512KB es el tamaño típico de sector para discos magnéticos


void initInputSystem(char *fileName);

void destroyInputSystem();

char nextCharacter();

void prepareForNextLex();

char previousCharacter();

#endif //PRACTICA1_INPUTSYSTEM_H
