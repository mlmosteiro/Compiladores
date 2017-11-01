#include "SintacticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include <stdio.h>


void initSintacticAnalyzer(char* filepath){
    initLexicalAnalyzer(filepath);

}

void startSintacticAnalisis(){
    lexemaOutput lexemaOut;
    while (lexemaOut.compLex != EOF) {
        lexemaOut = nextLexicalComponent();
        printf("[%d - %s]\n", lexemaOut.compLex, lexemaOut.lexema);
    }
}

void destroySintacticAnalyzer(){
    destroyLexicalAnalyzer();
}