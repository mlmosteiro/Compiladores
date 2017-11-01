#ifndef PRACTICA1_SYMBOLSTABLE_H
#define PRACTICA1_SYMBOLSTABLE_H
struct symbolStruct {
    char *lexema;
    int componenteLexico;
    int numLinea;
};

typedef struct symbolStruct symbolImput;

void initSymbolsTable();

void insert(char *lexema, int componenteLexico, int numLinea);

symbolImput search(char *lexema);

void modify();

void destroySymbolsTable();

void printSymbolsTable();

#endif //PRACTICA1_SYMBOLSTABLE_H
