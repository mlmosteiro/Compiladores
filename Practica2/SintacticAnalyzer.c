#include "SintacticAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "ErrorManager.h"
#include <stdio.h>

int init(char* path);
int yylex();
extern char* yytext;
FILE *yyin;

void initSintacticAnalyzer(char *filepath) {
    FILE *myfile = fopen(filepath, "r");
    if (!myfile) {
        showError(NOT_FILE_FOUNDED,0);
    }
    yyin = myfile;

}

void startSintacticAnalisis() {
    /* Pedimos el siguiente lexema al analizador léxico y lo procesamos.
     * En este caso unicamente lo imprimimos junto con el código del componente
     * lexico asociado
     */

    int n;
    n = yylex();
    while (n) {
        printf(" %d  %s\n", n, yytext);
        n = yylex();
    }
}

void destroySintacticAnalyzer() {
    destroyLexicalAnalyzer();
}