#include "SintacticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include <stdio.h>


void initSintacticAnalyzer(char *filepath) {
    initLexicalAnalyzer(filepath);

}

void startSintacticAnalisis() {
    lexemaOutput lexemaOut;
    lexemaOut = nextLexicalComponent();
    do {
        printf("[%d - %s]\n", lexemaOut.compLex, lexemaOut.lexema);
        lexemaOut = nextLexicalComponent();
    } while (lexemaOut.compLex != EOF);
}

void destroySintacticAnalyzer() {
    destroyLexicalAnalyzer();
}