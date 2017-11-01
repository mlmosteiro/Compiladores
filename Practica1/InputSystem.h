#ifndef PRACTICA1_INPUTSYSTEM_H
#define PRACTICA1_INPUTSYSTEM_H
#define SIZE_BLOCK 512  //512KB es el tamaño típico de sector para discos magnéticos


/* Función que inicia el sistema de entrada y todos los recursos necesarios */
void initInputSystem(char *fileName);

/* Función que libera todos los recursos reservados por el sistema de entrada*/
void destroyInputSystem();

/* Función que devuelve el siguiente caracter a leer*/
char nextCharacter();

/* Función que ajusta los punteros dentro del sistema de entrada para comenzar a leer otro lexema*/
void prepareForNextLex();

/* Función que devuelve el caracter anterior al que se estaba leyendo*/
char previousCharacter();

#endif //PRACTICA1_INPUTSYSTEM_H
