#include "BST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO: APLICAR GESTION DE ERRORES

struct nodo {
    tipoelem content;
    struct nodo *izq, *der;
};

typedef struct nodo* abb;

void initTree(abb *A) {
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
        *A = (abb) malloc(sizeof (struct nodo));
        (*A)->content = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(E.clave,(*A)->content.clave)<0)
        inserta(&(*A)->izq, E);
    else
        inserta(&(*A)->der, E);
}

tipoelem suprime_min(abb *A) {
    abb aux;
    tipoelem ele;
    if ((*A)->izq == NULL) {
        ele = (*A)->content;
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
        if (strcmp(E.clave,(*A)->content.clave)<0)
            suprime(&(*A)->izq, E);
        else if (strcmp(E.clave,(*A)->content.clave)>0)
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
            (*A)->content = suprime_min(&(*A)->der);

    }
}

unsigned existe(abb A, tipoelem E) {
    if (esVacio(A))
        return 0;
    else if (strcmp(E.clave,(A->content).clave)==0)
        return 1;
    else if (strcmp(E.clave,(A->content).clave)>0)
        return existe(A->der, E);
    else
        return existe(A->izq, E);
}

void getContent(abb A, tipoelem *E) {
    *E = A->content;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

void buscarNodo(abb A, tipoclave cl, tipoelem *res) {
    if (esVacio(A)) {
        *res = NULL;
        printf("Clave inexistente\n");
    }
    else if (strcmp(cl,(A->content).clave)==0) {
        *res = A->content;
    }
    else if (strcmp(cl,(A->content).clave)<0) {
        buscarNodo(izq(A), cl, res);
    }
    else{
        buscarNodo(der(A), cl, res);
    }
}

void modifica(abb *A, tipoclave cl, tipoelem nodo) {
    if (esVacio(*A)) {
        printf("Clave inexistente\n");
    } else if (strcmp(cl,((*A)->content).clave)==0)
        (*A)->content = nodo;
    else if (strcmp(cl,((*A)->content).clave)<0)
        modifica(&(*A)->izq, cl, nodo);
    else
        modifica(&(*A)->der, cl, nodo);
}