#include "BST.h"
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

void inserta(abb *A, tipoelem E) {
    if (esVacio(*A)) {
        *A = (abb) malloc(sizeof(struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(E.lexema, (*A)->info.lexema) < 0)
        inserta(&(*A)->izq, E);
    else
        inserta(&(*A)->der, E);
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
        if (strcmp(E.lexema, (*A)->info.lexema) < 0)
            suprime(&(*A)->izq, E);
        else if (strcmp(E.lexema, (*A)->info.lexema) > 0)
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
    else if (strcmp(E.lexema, (A->info).lexema) == 0)
        return 1;
    else if (strcmp(E.lexema, (A->info).lexema) > 0)
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

void buscaNodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (esVacio(A))
        nodo = NULL;
    else if (strcmp(cl, (A->info).lexema) == 0)
        *nodo = A->info;
    else if (strcmp(cl, (A->info).lexema) < 0)
        buscaNodo(izq(A), cl, nodo);
    else
        buscaNodo(der(A), cl, nodo);
}

void modifica(abb *A, tipoclave cl, tipoelem nodo) {
    if (esVacio(*A)) {
        printf("Clave inexistente\n");
    } else if (strcmp(cl, ((*A)->info).lexema) == 0)
        (*A)->info = nodo;
    else if (strcmp(cl, ((*A)->info).lexema) < 0)
        modifica(&(*A)->izq, cl, nodo);
    else
        modifica(&(*A)->der, cl, nodo);
}