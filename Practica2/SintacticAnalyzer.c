#include "SintacticAnalyzer.h"
#include "ErrorManager.h"
#include <stdio.h>

// Declaracion de variables y funciones correspondientes a las generadas por flex en lex.yy.c
int yylex();            // Funcion que nos devulve el componente lexico (entero)
extern char* yytext;    // Variable que contiene el lexema
FILE *yyin;             // Variable en la que se determina el archivo de entrada (codigo fuente)
FILE *file;
void initSintacticAnalyzer(char *filepath) {
    file = fopen(filepath, "r");
    if (!file) {
        showError(NOT_FILE_FOUNDED,0);
    }
    yyin = file;

}

void startSintacticAnalisis() {
    /* Pedimos el siguiente lexema al analizador léxico (lex.yy.c) y lo procesamos.
     * En este caso unicamente lo imprimimos junto con el código del componente
     * lexico asociado
     */

    int n;
    n = yylex();
    while (n) {   // Mientras n != 0, lo cual indicaria que se ha finalizado el analisis (EOF o forzado con yytermiante())
        printf("[%d - %s] \n", n, yytext);
        n = yylex();
    }
}

void destroySintacticAnalyzer() {
    fclose(file);
    file = NULL;
}