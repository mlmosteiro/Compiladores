#include <stdio.h>
#include "ErrorManager.h"


void showError(int errorCode, int numLinea) {
    printf("\033[0;31m");
    switch (errorCode) {
        case MISSING_ARGUMENTS:
            printf("Número de argumentos insuficiente.\nSintaxis: ./practica1 <archivoCodigoFuente> <archivoDefiniciones>");
            break;
        case NOT_KEYWORDS_FILE_FOUNDED:
            printf("No se ha encontrado el archivo de palabras clave especificado\n");
            break;
        case NOT_FILE_FOUNDED:
            printf("No se ha encontrado el archivo de código fuente especificado\n");
            break;
       case ILLEGAL_ESCAPED_CHARACTER:
            printf("[ERROR LEXICO - linea %d] \tCaracter escapado ilegal.\n", numLinea);
            break;
        case ILLEGAL_CHARACTER:
            printf("[ERROR LEXICO - linea %d] \tCaracter ilegal.\n", numLinea);
            break;
        default:
            printf("[ERROR]");
    }
    printf("\033[0m\n");

}