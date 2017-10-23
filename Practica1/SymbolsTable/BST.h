#ifndef PRACTICA1_BST_H
#define PRACTICA1_BST_H

#include "SymbolsTable.h"

typedef struct symbolStruct tipoelem;

typedef char* tipoclave;

typedef struct celda * abb;

//TODO: Preguntar porque tipo opaco

void crea(abb *A);
void destruye(abb *A);
unsigned esVacio(abb A);
void inserta(abb *A, tipoelem E);
tipoelem suprime_min(abb *A);
void suprime(abb *A,tipoelem E);
unsigned esMiembro(abb A, tipoelem E);
abb izq(abb A);
abb der(abb A);
void info(abb A, tipoelem *E);
void buscaNodo(abb A, tipoclave cl, tipoelem *nodo);
void modifica(abb *A, tipoclave cl, tipoelem nodo);

#endif //PRACTICA1_BST_H
