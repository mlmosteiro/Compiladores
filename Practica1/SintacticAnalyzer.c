#include "SintacticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "ErrorManager.h"
#include <stdio.h>

void initSintacticAnalyzer(char *filepath) {
    initLexicalAnalyzer(filepath);
}

void startSintacticAnalisis() {
    lexemaOutput lexemaOut;
    /* Pedimos el siguiente lexema al analizador léxico y lo procesamos.
     * En este caso unicamente lo imprimimos junto con el código del componente
     * lexico asociado
     */
    lexemaOut = nextLexicalComponent();
    do {
        printf("[%d - %s]\n", lexemaOut.compLex, lexemaOut.lexema);
        lexemaOut = nextLexicalComponent();
    } while (lexemaOut.compLex != EOF );
}

void destroySintacticAnalyzer() {
    destroyLexicalAnalyzer();
}