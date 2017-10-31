#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "LexicalAnalyzer.h"
#include "InputSystem.h"
#include "SymbolsTable/SymbolsTable.h"
#include "Definitions.h"
#include "ErrorManager.h"

#define INITIAL_STATE               900
#define NUMBER_NOT_DECIMAL_LIT      901
#define NUMBER_OCTAL_LIT            903
#define NUMBER_HEX_LIT              904
#define NUMBER_EXPONENTIAL_LIT      902
#define NUMBER_EXP_SIG              905
#define NUMBER_EXP_NO_SIG           906
#define IS_COMMENT                  908
#define GENERAL_COMMENT_END         909
#define START_OF_INTERPRETED_STRING 911
#define START_OF_RAW_STRING         912
#define LETTER                      913
#define GENERAL_COMMENT             914

//Todo: gestion de erroreS


int currentBufferIndex = - 1;
char *lexemaBuffer;
int mult = 1;
char character;
int numLinea = 0;
char operatorsArray[8] = {'-', '*', '^', '!', '%', '|', '\0'};
char separatorsArray[7] = {',', ';', '\n', '\0'}; // Tambien podriamos usar strpbrk(), pero no lo haremos.
lexemaOutput lexemaOut;

void initLexicalAnalyzer() {
    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_LA);
}

void destroyLexicalAnalyzer() {
    free(lexemaBuffer);
}

void extendLexemaBuffer() {
    lexemaBuffer = (char *) realloc(lexemaBuffer, ((mult ++) * sizeof(char) * SIZE_LA));
}

void getNextCharacter() {
    character = nextCharacter();
    currentBufferIndex ++;
    if (currentBufferIndex == SIZE_LA)
        extendLexemaBuffer();
    lexemaBuffer[currentBufferIndex] = character;
}

void undoNextCharacter() {
    lexemaBuffer[currentBufferIndex] = '\0';
    currentBufferIndex --;
    character = previousCharacter();
}

void restartLexemaBuffer() {
    currentBufferIndex = - 1;
    free(lexemaBuffer);
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_LA);
}

lexemaOutput lexemaFinded(int componenteLexico, bool toBeInserted) {

    if (toBeInserted) {
        undoNextCharacter();
        symbolImput isInSymbolTable = search(lexemaBuffer);
        if (isInSymbolTable.lexema == NULL) {
            insert(lexemaBuffer, componenteLexico, numLinea);
        } else {
            lexemaOut.lexema = isInSymbolTable.lexema;
            lexemaOut.compLex = isInSymbolTable.componenteLexico;
            return lexemaOut;
        }
    }
    lexemaOut.lexema = lexemaBuffer;
    lexemaOut.compLex = componenteLexico;
    return lexemaOut;
}

int automatonNumbers() {
    int stateNumber = INITIAL_STATE;
    undoNextCharacter();                                    //TODO: ARREGLAR ESTO.

    while (1) {
        getNextCharacter();

        switch (stateNumber) {
            case INITIAL_STATE:                                                     // En el estado inicial, si el primer caracter leido:
                if (character != '0' && (isdigit(character))) {                     // - es un digito y no es un cero
                    stateNumber = NUMBER_INTEGER_LIT;                               // podemos decir que, por ahora, se trata de un número entero (decimal)
                } else if (character == '0') {                                      // - es un dígito y es el cero
                    stateNumber = NUMBER_NOT_DECIMAL_LIT;                           // podemos decir que, por ahora, se trata de un número no decimal literal (hexadecimal, punto flotante, etc)
                } else if (character == '.') {                                      // - es un '.'
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else {
                    undoNextCharacter();
                    return NUMBER_INTEGER_LIT;
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
                    undoNextCharacter();
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
                    undoNextCharacter();
                    return NUMBER_INTEGER_LIT;                                      // Ya estamos leyendo otro lexema, lo que significa que hemos leido el digito '0' decim;
                }
                break;

            case NUMBER_OCTAL_LIT:                                                  // Si estamos en este estado, estamos seguros de que seguiremos leyendo un
                if (isdigit(character) && (character >= '0' || character < '8')) {  // numero octal
                    stateNumber = NUMBER_OCTAL_LIT;
                } else {                                                            // Si no se lee un caracter octal, hemos empezado a leer otro lexema distinto.
                    undoNextCharacter();
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_HEX_LIT:
                if (isdigit(character) || (character >= 'A' && character <= 'F')
                    || (character >= 'a' && character <=
                                            'f')) {                    // Si estamos en este estado, estamos seguros de que seguiremos leyendo
                    stateNumber = NUMBER_HEX_LIT;                                   // un numero hexadecimal
                } else {                                                            // Si no se lee un caracter hexadecimal, hemos empezado a leer otro lexema distinto.
                    undoNextCharacter();
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
                    undoNextCharacter();
                    return NUMBER_FLOAT_LIT;
                }
                break;

            case NUMBER_EXPONENTIAL_LIT://TODO: FIJAR QUE EL NUMERO NO PUEDE ACABAR SOLO EN E   // Si estamos en este estado, es porque hemos leido ya una 'e' en algun momento anterior,
                if ((isdigit(
                        character))) {                                         // entonces estamos seguros de que seguiremos leyendo un número con exponente
                    stateNumber = NUMBER_EXP_NO_SIG;
                } else if (character == '+' || character ==
                                               '-') {                  // Si leemos un signo '+' o '-' entonces hemos leido un exponente con signo (interesante para verificar formato correcto)
                    if (! isdigit(
                            character)) {                                 // Si el siguiente simbolo NO es un digito,  entonces el formato es incorrecto
                        //TODO: GESTOR DE ERRORES, SE ESPERABA UN DIGITO
                    } else {
                        stateNumber = NUMBER_EXP_SIG;                               // Si se lee un digito, entonces el formato YA es correcto, en este caso, exponente con Signo
                    }
                } else {
                    // TODO: GEstor de errores : error con formato erroneo
                    // printError(BAD_FORMATED_EXP, line, lexemaBuffer);            // En cualquier otro caso, el formato ES ERRONEO.
                    undoNextCharacter();
                    return NUMBER_FLOAT_LIT;
                }
                break;

            case NUMBER_EXP_SIG:
            case NUMBER_EXP_NO_SIG://TODO: CUIDADO CON EL CASO E+0i -> no se puede
                if (isdigit(character)) {
                    stateNumber = NUMBER_EXP_SIG;
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    undoNextCharacter();
                    return NUMBER_FLOAT_LIT;
                }
                break;

            case NUMBER_IMAGINARY_LIT:                                              // Cualquier cosa que leamos a continuacion ya es otro lexema
                undoNextCharacter();
                return NUMBER_IMAGINARY_LIT;
        }
    }
}

int automatonSeparators() {
    int i;
    switch (character) {
        case ':':
            getNextCharacter();
            if (character == '=') {
                return DECLARATION_ASSIG;
            } else {
                undoNextCharacter();
                return ':';
            }
        case '.':
            getNextCharacter();     //CASO EN EL QUE SE TRATE DE UN NUMERO EN PUNTO FLOTANTE
            if (isdigit(character)) {
                undoNextCharacter();
                return automatonNumbers();
            } else {
                undoNextCharacter();
                return '.';
            }
        default:
            i = 0;
            while (separatorsArray[i] != '\0') {
                if (character == separatorsArray[i]) {
                    if (character != '\n' && character != ' ') {
                        return separatorsArray[i];
                    } else if (character == '\n') {
                        numLinea ++;
                        restartLexemaBuffer();
                        strcpy(lexemaBuffer, "\\n");
                        return LINE_FEED;
                    } else if (character == ' ') {
                        return ' ';
                    }
                }
                i ++;
            }
    }
    return NOT_FINAL_STATE;
}

int automatonOperators() {  // Se encarga de identificar cualquier tipo de operador
    int i;
    switch (character) {
        case '=':
            getNextCharacter();
            if (character != '=') {
                undoNextCharacter();
                return ASSIGNATION;
            } else if (character == '=') {
                return EQUALS;
            }
            break;

        case '<':
            getNextCharacter();
            if (character == '=') {
                return LESS_EQUAL;
            } else if (character == '-') {
                return RECEIVE;
            } else if (character == '<') {
                return LESS_LESS;
            } else {
                undoNextCharacter();
                return '<';
            }

        case '>':
            getNextCharacter();
            if (character == '=') {
                return GREATER_EQUAL;
            } else if (character == '>') {
                return GREATER_GREATER;
            } else {
                undoNextCharacter();
                return '>';
            }

        case '+':
            getNextCharacter();
            if (character == '=') {
                return PLUS_EQUAL;
            } else if (character == '+') {
                return PLUS_PLUS;
            } else {
                undoNextCharacter();
                return '+';
            }
        case '/': //En este caso podriamos modificar esto para identificar operadores como /=
            getNextCharacter();
            return '/';
        default:
            i = 0;
            while (operatorsArray[i] != '\0') {
                if (character == operatorsArray[i]) {
                    return operatorsArray[i];
                }
                i ++;
            }
    }
    return NOT_FINAL_STATE;
}

int automatonComments() {
    int stateComment = INITIAL_STATE;
    while (1) {
        if (character == EOF) {
            undoNextCharacter();
            //TODO: ERROR: El comentario no está cerrado.
            //printError(COMMENT_DOES_NOT_END,line,buffer);
            return EOF;
        }

        if (character == '\n') {//TODO: TENER CUIDADO CONESTO
            numLinea ++;
        }

        switch (stateComment) {
            case INITIAL_STATE:
                if (character == '/') { //First char of a comment
                    stateComment = IS_COMMENT;
                } else {
                    return NOT_FINAL_STATE;
                    //TODO GESTOR DE ERRORES
                }
                break;

            case IS_COMMENT:
                getNextCharacter();
                if (character == '*') { //General comment /*
                    stateComment = GENERAL_COMMENT;
                } else if (character == '/') {// Line Comment
                    getNextCharacter();
                    while (character != '\n') {
                        character = nextCharacter();
                    }
                    return COMMENT;
                } else { //Se trata del operador '/'
                    undoNextCharacter();
                    undoNextCharacter();
                    return automatonOperators();
                }
                break;

            case GENERAL_COMMENT:
                while (character != '*') {//Multiple comment /*
                    character = nextCharacter();
                }
                stateComment = GENERAL_COMMENT_END;
                break;
            case GENERAL_COMMENT_END:
                getNextCharacter();
                if (character == '/') {  // Multiple comment */
                    return COMMENT;
                } else {
                    stateComment = GENERAL_COMMENT;
                }
                break;
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
    return NOT_FINAL_STATE;
}

int automatonStrings() {
    int state = INITIAL_STATE;
    while (1) {
        switch (state) {
            case INITIAL_STATE:
                if (character == '"') {
                    state = START_OF_INTERPRETED_STRING;
                } else if (character == '`') {
                    state = START_OF_RAW_STRING;
                } else {
                    return NOT_FINAL_STATE;
                }
                break;
            case START_OF_INTERPRETED_STRING:
                getNextCharacter();
                while (character != '"') {
                    character = nextCharacter();
                    if (character == '\n') { //TODO: CHECAR SI ESTO FALLA
                        numLinea ++;
                    } else if (character == '\\') {
                        if (nextCharacter() == '"') {
                            continue;
                        } else {
                            undoNextCharacter();
                        }
                    } else if (character == EOF) {
                        //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                        //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                        return EOF;
                    }
                }
                return STRING_LIT;

            case START_OF_RAW_STRING:
                getNextCharacter();
                while (character != '`') {
                    character = nextCharacter();

                    if (character == '\n') { //TODO: CHECAR SI ESTO FALLA
                        numLinea ++;
                    } else if (character == EOF) {
                        //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                        //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                        return EOF;
                    }
                }
                return STRING_LIT;
        }
    }
}

lexemaOutput nextLexicalComponent() {
    character = ' ';
    restartLexemaBuffer();
    int state = INITIAL_STATE;

    while (1) {
        getNextCharacter();
        switch (state) {
            case INITIAL_STATE:
                //END OF FILE
                if (character == EOF) {
                    return lexemaFinded(EOF, false);
                }
                    //This else analyze separators, comments, operators, brackets and strings
                else if (! isalpha(character) && ! isdigit(character)) {
                    int finded;
                    finded = automatonSeparators();
                    if (finded == NOT_FINAL_STATE) {
                        finded = automatonComments();
                    }
                    if (finded == NOT_FINAL_STATE) {
                        finded = automatonOperators();
                    }
                    if (finded == NOT_FINAL_STATE) {
                        finded = automatonBrackets();
                    }
                    if (finded == NOT_FINAL_STATE) {
                        finded = automatonStrings();
                    }
                    if (finded == NOT_FINAL_STATE || finded == COMMENT) {
                        restartLexemaBuffer();
                        break;
                        //Todo gestor DE ERRORES CARACTER NO ESPERADO
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
                } else if (isalpha(character) || character == '_') {
                    state = LETTER;
                }
                break;
        }
    }
}


