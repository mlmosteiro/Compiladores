#include "BST.h"

#ifndef PRACTICA1_SYMBOLSTABLE_H
#define PRACTICA1_SYMBOLSTABLE_H

struct symbolStruct{
    char* lexema;
    int componenteLexico;
    char* clave = lexema;

};

typedef struct symbolStruct symbolImput;

void initSymbolsTable();

void insert(char* lexema, int componenteLexico);

int search(char* lexema);

void modify();

void destroySymbolsTable();

#endif //PRACTICA1_SYMBOLSTABLE_H
