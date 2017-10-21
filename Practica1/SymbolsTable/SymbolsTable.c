#include "SymbolsTable.h"

abb* symbolsTable;

void importLenguageKeyWords(){

}

void initSymbolsTable(){
    initTree(symbolsTable);
    importLenguageKeyWords();

}

