#ifndef PRACTICA1_BST_H
#define PRACTICA1_BST_H

#include <stdbool.h>

////////////////////////////////////////////////////////////////
struct lexemaStruct{
    char* lexema;
    int componenteLexico;
};

typedef struct lexemaStruct tipoelem;

typedef char* tipoclave;

////////////////////////////////////////////////////////////////

//typedef void *abb; //tipo opaco

typedef struct nodo* abb;

void initTree(abb *A);
void destruye(abb *A);
bool esVacio(abb A);
void inserta(abb *A, tipoelem E);
void suprime(abb *A,tipoelem E);
unsigned existe(abb A, tipoelem E);
void getContent(abb A, tipoelem *E);
void buscarNodo(abb A, tipoclave cl, tipoelem *nodo);
void modifica(abb *A, tipoclave cl, tipoelem nodo);
#endif //PRACTICA1_BST_H
