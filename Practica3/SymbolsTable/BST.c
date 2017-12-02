#include "BST.h"
#include "SymbolsTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct celda {
    tipoelem info;
    struct celda *izq, *der;
};


void crea(abb *A) {
    *A = NULL;
}

void destruye(abb *A) {
    if (*A != NULL) {
        destruye(&(*A)->izq);
        destruye(&(*A)->der);
        free(*A);
        *A = NULL;
    }
}

bool esVacio(abb A) {
    return A == NULL;
}

tipoelem * inserta(abb *A, tipoelem E) {
    if (esVacio(*A)) {
        *A = (abb) malloc(sizeof(struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return &(*A)->info;
    } else if (strcmp(E.name, (*A)->info.name) < 0)
        return  inserta(&(*A)->izq, E);
    else
        return inserta(&(*A)->der, E);
}

tipoelem suprime_min(abb *A) {
    abb aux;
    tipoelem ele;
    if ((*A)->izq == NULL) {
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else
        return suprime_min(&(*A)->izq);

}

void suprime(abb *A, tipoelem E) {
    abb aux;
    if (*A != NULL) {
        if (strcmp(E.name, (*A)->info.name) < 0)
            suprime(&(*A)->izq, E);
        else if (strcmp(E.name, (*A)->info.name) > 0)
            suprime(&(*A)->der, E);
        else if ((*A)->izq == NULL && (*A)->der == NULL) {
            free(*A);
            *A = NULL;
        } else if ((*A)->izq == NULL) {
            aux = *A;
            *A = (*A)->der;
            free(aux);
        } else if ((*A)->der == NULL) {
            aux = *A;
            *A = (*A)->izq;
            free(aux);
        } else
            (*A)->info = suprime_min(&(*A)->der);

    }
}

bool esMiembro(abb A, tipoelem E) {
    if (esVacio(A))
        return 0;
    else if (strcmp(E.name, (A->info).name) == 0)
        return 1;
    else if (strcmp(E.name, (A->info).name) > 0)
        return esMiembro(A->der, E);
    else
        return esMiembro(A->izq, E);
}

void info(abb A, tipoelem *E) {
    *E = A->info;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

tipoelem * buscaNodo(abb A, tipoclave cl) {
    if (esVacio(A))
        return NULL;
    else if (strcmp(cl, (A->info).name) == 0)
        return &(A->info);
    else if (strcmp(cl, (A->info).name) < 0)
        return buscaNodo(izq(A), cl);
    else
        return buscaNodo(der(A), cl);
}


void modifica(abb *A, tipoclave cl, tipoelem nodo) {
    if (esVacio(*A)) {
        printf("Clave inexistente\n");
    } else if (strcmp(cl, ((*A)->info).name) == 0)
        (*A)->info = nodo;
    else if (strcmp(cl, ((*A)->info).name) < 0)
        modifica(&(*A)->izq, cl, nodo);
    else
        modifica(&(*A)->der, cl, nodo);
}