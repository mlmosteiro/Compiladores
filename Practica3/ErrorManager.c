#include <stdio.h>
#include "ErrorManager.h"


#define MISSING_ARGUMENTS 			(-101)
#define NOT_FILE_FOUNDED 			(-103)
#define NOT_INITIALIZED_VARIABLE	(-104)
#define OVERWITE_VARIABLE 			(-105)
#define ILLEGAL_CHARACTER 			(-107)
#define NOT_FUNCTION	 			(-108)
#define DIVISION_BY_ZERO			(-109)
#define VALUE_OF_FUNCTION			(-110)


void showError(int errorCode, int numLinea) {
    printf("\033[0;31m");
    switch (errorCode) {
        case NOT_FILE_FOUNDED:
            printf("No se ha encontrado el archivo de código fuente especificado");
            break;

        case VALUE_OF_FUNCTION:
            printf("[ERROR] No se puede mostrar el valor de una función");
            break;

        case NOT_INITIALIZED_VARIABLE:
            printf("[ERROR] Esa variable no está inicializada");
            break;
        case OVERWITE_VARIABLE:
            printf("[ERROR] Se está intentando asignar un valor a una función sobreescribir una constante.");
            break;
        case NOT_FUNCTION:
            printf("[ERROR] Función no existente");
            break;
        case ILLEGAL_CHARACTER:
            printf("[ERROR] Carácter ilegal.");
            break;
        case DIVISION_BY_ZERO:
            printf("[ERROR] División por cero");
            break;
        default:
            printf("[ERROR]");
    }
    printf("\033[0m\n");

}