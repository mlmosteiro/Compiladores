#include "Definitions.h"
#include "InputSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "ErrorManager.h"

#define INITIAL_STATE                   900
#define NUMBER_NOT_DECIMAL_LIT          901
#define NUMBER_EXPONENTIAL_LIT          902
#define NUMBER_EXP_SIG                  903
#define NUMBER_HEX_LIT                  904
#define NUMBER_OCTAL_LIT                905
#define BEGIN_COMMENT                   906
#define IS_COMMENT                      907
#define GENERAL_COMMENT_END             908
#define POTENTIAL_GENERAL_COMMENT       909
#define START_OF_INTERPRETED_STRING     910
#define START_OF_RAW_STRING             911
#define LETTER                          912

char operatorsArray[8] = { '-', '*', '/', '^', '!', '%', '|', '\0'};
char separatorsArray[5] = {',', ';', '.', '\n', '\0'};

//Todo: gestion de erroreS

char *lexemaBuffer;
int currentBufferIndex = -1;
int mult = 1;
char character;
int numLinea = 0;
lexemaOutput lexemaOut;

void initLexicalAnalyzer() {
    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

void extendLexemaBuffer() {
    lexemaBuffer = (char *) realloc(lexemaBuffer, ((mult ++) * sizeof(char) * SIZE_SA));
}

void restartLexemaBuffer() {
    free(lexemaBuffer);
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

lexemaOutput lexemaFinded(int componenteLexico, bool toBeInserted) {
    lexemaBuffer[currentBufferIndex] = '\0';
    previousCharacter();

    if (toBeInserted) {
        symbolImput *isInSymbolTable = search(lexemaBuffer);
        if (isInSymbolTable == NULL) {
            insert(lexemaBuffer, componenteLexico, numLinea);
        }
    }
    lexemaOut.lexema = lexemaBuffer;
    lexemaOut.compLex = componenteLexico;
    return  lexemaOut;
}

int automatonSeparators() {
    int i;
    switch (character) {
        case ':':
            character = nextCharacter();
            if (character == '=') {
                return DECLARATION_ASSIG;
            } else {
                character = previousCharacter();
                return ':';
            }
        default:
            i= 0;
            while (separatorsArray[i] != '\0') {
                if (character == separatorsArray[i]) {
                    if (character != '\n' && character != ' ') {
                        return separatorsArray[i];
                    } else if (character == '\n') {
                        numLinea ++;
                    }
                }
                i ++;
            }
    }
    return NULL;
}

int automatonComments() {
    int stateComment = BEGIN_COMMENT;
    while (1) {
        if (character == EOF) {
            previousCharacter();
            //TODO: ERROR: El comentario no está cerrado.
            //printError(COMMENT_DOES_NOT_END,line,buffer);
            return EOF;
        }

        if (character == '\n') {
            numLinea ++;
        }

        switch (stateComment) {
            case BEGIN_COMMENT:
                if (character == '/') { //First char of a comment
                    stateComment = IS_COMMENT;
                } else {
                    return - 1;
                    //TODO GESTOR DE ERRORES
                }
                break;

            case IS_COMMENT:
                if (character == '*') { //General comment /*
                    stateComment = POTENTIAL_GENERAL_COMMENT;
                } else if (character == '/') {// Line Comment
                    nextCharacter();
                    if (character == '/') {
                        while (character != '\n') {
                            nextCharacter();
                        }
                        return LINE_COMMENT;
                    }
                } else {
                    //TODO: PRIMERO CHECAR SI SE TRATA DE UN COMENTARIO, Y LUEGO DEL OPERANDO DIVISION. O TRATAR EL OPERANDO AQUI.
                }

                break;
            case POTENTIAL_GENERAL_COMMENT:
                if (character == '*') { //Multiple comment /*
                    stateComment = GENERAL_COMMENT_END;
                }
                break;
            case GENERAL_COMMENT_END:
                if (character == '/') {  // Multiple comment */
                    return GENERAL_COMMENT;
                } else {
                    stateComment = POTENTIAL_GENERAL_COMMENT;
                }
                break;
        }
        nextCharacter();
    }
}

int automatonOperators() {  // Se encarga de identificar cualquier tipo de operador
    int i;
    switch (character) {
        case '=':
            character = nextCharacter();
            //TODO: Aqui tener cuidado con la diferencia entre := e ==
            if (character != '=') {
                character = previousCharacter();
                return ASSIGNATION;
            } else if (character == '=') {
                return EQUALS;
            }
            break;

        case '<':
            character = nextCharacter();
            if (character == '=') {
                return LESS_EQUAL;
            } else if (character == '-') {
                return RECEIVE;
            } else if (character == '<') {
                return LESS_LESS;
            } else {
                character = previousCharacter();
                return '<';
            }

        case '>':
            character = nextCharacter();
            if (character == '=') {
                return GREATER_EQUAL;
            } else if (character == '>') {
                return GREATER_GREATER;
            } else {
                character = previousCharacter();
                return '>';
            }

        case '+':
            character = nextCharacter();
            if (character == '=') {
                return PLUS_EQUAL;
            } else if (character == '+') {
                return PLUS_PLUS;
            } else {
                character = previousCharacter();
                return '+';
            }

        default:
            i = 0;
            while (operatorsArray[i] != '\0') {
                if (character == operatorsArray[i]) {
                    return operatorsArray[i];
                }
                i ++;
            }
    }
}

int automatonBrackets() { // Se encarga de identificar cualquier tipo de paréntesis
    if (character == '(') {
        return '(';
    } else if (character == ')') {
        return ')';
    } else if (character == '[') {
        return '[';
    } else if (character == ']') {
        return ']';
    } else if (character == '{') {
        return '{';
    } else if (character == '}') {
        return '}';
    }
    return NULL;
}

int automatonStrings() {
    int state = INITIAL_STATE;
    while (1) {
        switch (state) {
            case INITIAL_STATE:
                if (character == '"' ) {
                    state = START_OF_INTERPRETED_STRING;
                } else if (character == '`') {
                    state = START_OF_RAW_STRING;
                } else{
                    return NULL;
                }
                break;
            case START_OF_INTERPRETED_STRING:
                if (character == '"') {
                    return STRING;
                } else if (character == '\n') { //TODO: CHECAR SI ESTO FALLA
                    numLinea ++;
                } else if (character == EOF) {
                    //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                    //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                    return EOF;
                }
                break;
            case START_OF_RAW_STRING:
                if (character == '`') {
                    return STRING;
                } else if (character == '\n') { //TODO: CHECAR SI ESTO FALLA
                    numLinea ++;
                } else if (character == EOF) {
                    //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                    //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                    return EOF;
                }
                break;
        }
        character = nextCharacter();
    }
}

int automatonNumbers() {
    int stateNumber = INITIAL_STATE;
    while (1) {
        character = nextCharacter();

        switch (stateNumber) {
            case INITIAL_STATE:                                                     // En el estado inicial, si el primer caracter leido:
                if (character != '0' && (isdigit(character))) {                     // - es un digito y no es un cero
                    stateNumber = NUMBER_INTEGER_LIT;                               // podemos decir que, por ahora, se trata de un número entero (decimal)
                } else if (character == '0') {                                      // - es un dígito y es el cero
                    stateNumber = NUMBER_NOT_DECIMAL_LIT;                           // podemos decir que, por ahora, se trata de un número no decimal literal (hexadecimal, punto flotante, etc)
                } else if (character == '.') {                                      // - es un '.'
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else {
                    // TODO: Gestor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_NUMBER, line, lexemaBuffer);          // hemos leido algo que NO forma parte de un número
                }
                break;

            case NUMBER_INTEGER_LIT:                                                // Habiendo leido por lo menos un digito decimal, y a continuacion leemos:
                if (isdigit(character)) {                                           // - un digito decimal
                    stateNumber = NUMBER_INTEGER_LIT;                               // podemos decir que, por ahora, se sigue tratando de un número decimal
                } else if (character == '.') {                                      // - un '.',
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // - una 'e'
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que, por ahora, se trata de un número con exponente (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // - una 'i'
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    // TODO: Gestor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_NUMBER, line, lexemaBuffer);          // hemos leido algo que NO forma parte de un número, ya es el siguiente lexema
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_NOT_DECIMAL_LIT:                                            // Habiendo leido por lo menos un digito entero no decimal, y acontinuación leemos:
                if (character == '.') {                                             // - un '.'
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else if (isdigit(character) && (character >= '0' || character < '8')) { // - un dígito octal
                    stateNumber = NUMBER_OCTAL_LIT;                                 // podemos decir que, por ahora, se trata de un número octal
                } else if (character == 'x' || character == 'X') {                  // - una 'x'
                    stateNumber = NUMBER_HEX_LIT;                                   // podemos decir que, por ahora, se trata de un número hexadecimal
                } else if (character == 'i') {                                      // - una 'i'
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    return NUMBER_INTEGER_LIT;                               // Ya estamos leyendo otro lexema, lo que significa que hemos leido el digito '0' decim;
                }
                break;

            case NUMBER_OCTAL_LIT:                                                  // Si estamos en este estado, estamos seguros de que seguiremos leyendo un
                if (isdigit(character) && (character >= '0' || character < '8')) {  // numero octal
                    stateNumber = NUMBER_OCTAL_LIT;
                } else {                                                            // Si no se lee un caracter octal, hemos empezado a leer otro lexema distinto.
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_HEX_LIT:
                if (isdigit(character) || character >= 'A' ||character >= 'a'       // Si estamos en este estado, estamos seguros de que seguiremos leyendo
                    || character <= 'F' || character <= 'f') {                      // un numero hexadecimal
                    stateNumber = NUMBER_HEX_LIT;
                } else {                                                            // Si no se lee un caracter hexadecimal, hemos empezado a leer otro lexema distinto.
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_FLOAT_LIT: // Números en punto flotante
                if (isdigit(
                        character)) {                                               // Si estamos en este estado, es porque hemos leido ya un '.' en algun momento anterior,
                    stateNumber = NUMBER_FLOAT_LIT;                                 // entonces estamos seguros de que seguiremos leyendo un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // Si leemos una 'e',
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que se ha leido un número con exponente  (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {                                                            // En cualquier otro caso, hemos empezado a leer otro lexema distinto
                    // TODO: GEstor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_DOUBLE, line, lexemaBuffer);
                    return NUMBER_FLOAT_LIT;
                }
                break;

            case NUMBER_EXPONENTIAL_LIT://TODO: FIJAR QUE EL NUMERO NO PUEDE ACABAR SOLO EN E   // Si estamos en este estado, es porque hemos leido ya una 'e' en algun momento anterior,
                if ((isdigit(
                        character))) {                                              // entonces estamos seguros de que seguiremos leyendo un número con exponente
                    stateNumber = NUMBER_EXPONENTIAL_LIT;
                } else if (character == '+' ||
                           character == '-') {                                      // Si leemos un signo '+' o '-' entonces hemos leido
                    stateNumber = NUMBER_EXP_SIG;                                   // un exponente con signo (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    // TODO: GEstor de errores : error con formato erroneo
                    // printError(BAD_FORMATED_EXP, line, lexemaBuffer);            // En cualquier otro caso, el formato ES ERRONEO.
                    return NUMBER_FLOAT_LIT;
                }
                break;
            case NUMBER_EXP_SIG:                                                    // Si estamos en este estado, es porque hemos leido ya un exponente con signo '+' o '-'
                if (isdigit(
                        character)) {                                               // Ahora solo se seperan digitos decimales
                    stateNumber = NUMBER_EXP_SIG;
                } else {                                                            // En cualquier otro caso, el formato ES ERRONEO

                }
                break;

            case NUMBER_IMAGINARY_LIT:                                              // Cualquier cosa que leamos a continuacion ya es otro lexema
                return NUMBER_IMAGINARY_LIT;
        }
    }
}

void destroyLexicalAnalyzer() {
    free(lexemaBuffer);
}

lexemaOutput nextLexicalComponent() {
    character = ' ';
    restartLexemaBuffer();
    int state = INITIAL_STATE;

    while (1) {
        nextCharacter();
        switch (state) {
            case INITIAL_STATE:
                //END OF FILE
                if (character == EOF) {
                    return lexemaFinded(EOF, false);
                }
                    //This else analyze separators, comments, operators, brackets and strings
                else if (! isalpha(character) && ! isdigit(character)) {
                    int finded = 0;
                    finded = automatonSeparators();
                    if (finded == NULL) {
                        finded = automatonComments();
                    }
                    if (finded == NULL) {
                        finded = automatonOperators();
                    }
                    if (finded == NULL) {
                        finded = automatonBrackets();
                    }
                    if (finded == NULL) {
                        finded = automatonStrings();
                    }
                    if (finded == NULL) {
                        //TODO_GESTOR DE ERRORES CARACTER NO ESPERADO
                    }
                    return lexemaFinded(finded, false);
                } else if (isdigit(character)) { //START BY DIGIT
                    return lexemaFinded(automatonNumbers(), false);
                } else if (isalpha(character) || character == '_') { //START BY LETTER
                    state = LETTER;
                }
                break;

            case LETTER: //LETTER (we arrive here only with a previous LETTER read)
                if (! isalpha(character) && ! isdigit(character) && character != '_') {
                    return lexemaFinded(IDENTIFIER, true);
                } else if (isalpha(character) | character == '_') {
                    state = LETTER;
                }
                break;
        }
    }
}
