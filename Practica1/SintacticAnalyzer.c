#include "SintacticAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "LexicalAnalyzer.h"


void initSintacticAnalyzer(){
    symbolImput* lexicalComponent;
    initLexicalAnalyzer();

}

void startSintacticAnalisis(){
    char *componenteLexico;
    componenteLexico = nextLexicalComponent();
}

void destroySintacticAnalyzer(){

    destroyLexicalAnalyzer();
}