#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"
#include "Definitions.h"
#include "ErrorManager.h"

#define INITIAL_STATE 				900
#define NUMBER_NOT_DECIMAL_LIT 		901
#define NUMBER_OCTAL_LIT 			902
#define NUMBER_HEX_LIT 				903
#define NUMBER_EXPONENTIAL_LIT 		904
#define NUMBER_EXP_SIG 				905
#define NUMBER_EXP_NO_SIG 			906
#define IS_COMMENT 					907
#define GENERAL_COMMENT_END 		908
#define START_OF_INTERPRETED_STRING 909
#define START_OF_RAW_STRING 		910
#define LETTER 						911
#define GENERAL_COMMENT 			912

char *lexemaBuffer;             // Buffer en el que construiremos el lexema
int currentBufferIndex = - 1;   // Variable que permitirá hacer una copia de la memoria al buffer en el construimos el lexema
int mult = 1;                   // Variable que nos permite redimencionar el buffer en el que se construye el lexema
char character;                 // Variable en la que almacenamos el caracter que nos devuelve el sistema de entrada
int numLinea = 1;               // Contador que nos indica la linea actual en el código fuente que se está leyendo

/* Arrays en los que establecemos los parenresis, operadores, separadores o caracteres que se pueden escapar*/
char bracketsArray[7]={'(',')','[',']','{','}','\0'};
char operatorsArray[7] = {'-', '*', '^', '!', '%', '|', '\0'};
char separatorsArray[4] = {',', ';', '\n', '\0'}; // Tambien podriamos usar strpbrk(), pero no lo haremos.
char escapedChar[11] = {'n', 'a', 'b', 'f', 'r', 't', 'v', '\\', '\"', '\'', '\0'};

void initLexicalAnalyzer(char *filePath) {
    initInputSystem(filePath);
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_LA);
}

void destroyLexicalAnalyzer() {
    free(lexemaBuffer);
    destroyInputSystem();
}

/* Función que redimensiona el buffer en el que se construye el sistema en caso de que se haya superado su capacidad */
void extendLexemaBuffer() {
    lexemaBuffer = (char *) realloc(lexemaBuffer, ((mult ++) * sizeof(char) * SIZE_LA));
}

/* Funcion que reincia el buffer, liberando la memoria en caso de que se hubiese redimensionado el buffer */
void restartLexemaBuffer() {
    currentBufferIndex = - 1;
    if(mult>1) {
        free(lexemaBuffer);
        lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_LA);
        mult=1;
    } else{
        memset(lexemaBuffer, '\0', sizeof (char)*SIZE_LA);
    }
}

/* Función interna con la que, además de pedir el siguiente caracter al sistema de entrada, realizamos las otras
 * operaciones necesarias para poder ir construyendo el lexema
 */
void getNextCharacter() {
    character = nextCharacter();
    currentBufferIndex ++;
    if (currentBufferIndex == SIZE_LA){ // Desbordamiento del buffer
        extendLexemaBuffer();
    }
    lexemaBuffer[currentBufferIndex] = character;

    // En caso de que el lexema actual sea tan grande como un bloque del sistema de entrada,
    // entonces se ha superado el tamaño de lexema permitido
    if (currentBufferIndex == SIZE_BLOCK) {
        showError(ILLEGAL_LEXEMA_SIZE, numLinea);
        restartLexemaBuffer();
        return;
    }
}

/* Función interna que, además de retroceder el caracter actual a leer en el sistema de entrada, realiza
 * las operaciones pertienentes en el buffer del lexema para construirlo correctamente
 */
void undoNextCharacter() {
    lexemaBuffer[currentBufferIndex] = '\0';
    currentBufferIndex --;
    character = previousCharacter();
}

/* Función que devuelve el lexema actual (que se encuentra en el buffer, asociandolo al componente lexico
 * indicado como parametro.
 *
 * En caso de que el flag toBeInserted este activado, se verifica si ya está en la tabla de símbolos, en cuyo
 * caso se obtiene la información correspondiente al componente lexico de la tabla. Si no se encuentra en la tabla
 * entonces se trata de un identificador nuevo, por lo que se inserta.
 */
lexemaOutput lexemaFinded(int componenteLexico, bool toBeInserted) {
    lexemaOutput lexemaOut;

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
    prepareForNextLex();
    return lexemaOut;
}

// Autómata encargado de identificar numeros enteros, en punto flotante o imagianrios.
int automatonNumbers() {
    int stateNumber = INITIAL_STATE;
    while (1) {
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
                }
                break;

            case NUMBER_INTEGER_LIT:                                                // Habiendo leido por lo menos un digito decimal, y a continuacion leemos:
                getNextCharacter();
                if (isdigit(character)) {                                           // - un digito decimal
                    stateNumber = NUMBER_INTEGER_LIT;                               // podemos decir que, por ahora, se sigue tratando de un número decimal
                } else if (character == '.') {                                      // - un '.',
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // - una 'e'
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que, por ahora, se trata de un número con exponente (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // - una 'i'
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    undoNextCharacter();                                            // hemos leido algo que NO forma parte de un número, ya es el siguiente lexema
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_NOT_DECIMAL_LIT:                                            // Habiendo leido por lo menos un digito entero no decimal, y acontinuación leemos:
                getNextCharacter();
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
                getNextCharacter();
                if (isdigit(character) && (character >= '0' || character < '8')) {  // numero octal
                    stateNumber = NUMBER_OCTAL_LIT;
                } else {                                                            // Si no se lee un caracter octal, hemos empezado a leer otro lexema distinto.
                    undoNextCharacter();
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_HEX_LIT:
                getNextCharacter();
                if (isdigit(character) || (character >= 'A' && character <= 'F')
                    || (character >= 'a' && character <='f')) {                     // Si estamos en este estado, estamos seguros de que seguiremos leyendo
                    stateNumber = NUMBER_HEX_LIT;                                   // un numero hexadecimal
                } else {                                                            // Si no se lee un caracter hexadecimal, hemos empezado a leer otro lexema distinto.
                    undoNextCharacter();
                    return NUMBER_INTEGER_LIT;
                }
                break;

            case NUMBER_FLOAT_LIT:
                getNextCharacter();
                if (isdigit(
                        character)) {                                               // Si estamos en este estado, es porque hemos leido ya un '.' en algun momento anterior,
                    stateNumber = NUMBER_FLOAT_LIT;                                 // entonces estamos seguros de que seguiremos leyendo un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // Si leemos una 'e',
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que se ha leido un número con exponente  (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {                                                            // En cualquier otro caso, hemos empezado a leer otro lexema distinto
                    undoNextCharacter();
                    return NUMBER_FLOAT_LIT;
                }
                break;

            case NUMBER_EXPONENTIAL_LIT:                                            // Si estamos en este estado, es porque hemos leido ya una 'e' en algun momento anterior,
                getNextCharacter();
                if ((isdigit(
                        character))) {                                              // entonces estamos seguros de que seguiremos leyendo un número con exponente
                    stateNumber = NUMBER_EXP_NO_SIG;
                } else if (character == '+' || character == '-') {
                    // Si leemos un signo '+' o '-' entonces hemos leido un exponente con signo (interesante para verificar formato correcto)
                    character = nextCharacter();
                    if (! isdigit(
                            character)) {                                     // Si el siguiente simbolo NO es un digito,  entonces el formato es incorrecto
                        character = previousCharacter();
                        undoNextCharacter();                                        // Si a continuacion del simbolo + no hay un digito,
                        undoNextCharacter();
                        return NUMBER_FLOAT_LIT;                                    // entonces se trata solo de un numero en punto flotante
                    } else {
                        character = previousCharacter();
                        stateNumber = NUMBER_EXP_SIG;                               // Si se lee un digito, entonces el formato YA es correcto, en este caso, exponente con Signo
                    }
                } else {
                    undoNextCharacter();                                            // En cualquier otro caso, el formato ES ERRONEO para numero con explonente.
                    undoNextCharacter();
                    return NUMBER_FLOAT_LIT;                                        // devolvemos el numero flotante.
                }
                break;

            case NUMBER_EXP_SIG:
            case NUMBER_EXP_NO_SIG:
                getNextCharacter();
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
                return NUMBER_IMAGINARY_LIT;
        }
    }
}

// Autómata encargado de identificar los separadores
int automatonSeparators() {
    /* En esta función hay que tener especial cuidado con los separadores que pueden ser un lexema por
     * si solos oformar parte de uno más grande
     */
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
            getNextCharacter();
            if (isdigit(character)) {  //Si el siguiente caracter a leer es un digito, se trata de un numero
                undoNextCharacter();
                return automatonNumbers(); // redirigimos al autómata que reconoce números
            } else {
                undoNextCharacter();
                return '.';
            }

        // Si no se trata de ninguno de los casos especiales de arriba, recorremos el array de separadores.
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

// Autómata encargado de identificar los operadores
int automatonOperators() {
    /* En esta función hay que tener especial cuidado con los operadores que pueden ser un lexema por
     * si solos oformar parte de uno más grande
     */
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

        // Si no se trata de ninguno de los casos especiales de arriba, recorremos el array de operadores.
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

// Autómata encargado de identificar los comentarios
int automatonComments() {
    int stateComment = INITIAL_STATE;
    int flagLineFeed = 0;
    while (1) {

        // En caso de que lleguemos aun EOF quiere decir que hemos leido
        // tod0 el archivo y NO se ha cerrado el comentario correctamente
        if (character == EOF) {
            undoNextCharacter();
            showError(NOT_CLOSED_COMMENT, numLinea);
            return EOF;
        }

        switch (stateComment) {
            case INITIAL_STATE:
                // Si el caracter que leemos es un '/' entonces puede ser el incio de un comentario
                if (character == '/') {
                    stateComment = IS_COMMENT;
                } else {
                    return NOT_FINAL_STATE;
                }
                break;

            case IS_COMMENT:
                // Si el caracter que leemos es un '*' entonces puede ser el incio de un comentario multilinea (general)
                if (character == '*') { //General comment /*
                    stateComment = GENERAL_COMMENT;
                } else if (character == '/') {
                // Si el caracter que leemos es un '/' entonces puede ser el incio de un comentario en linea
                    getNextCharacter();
                    while (character != '\n') {
                        character = nextCharacter();
                    }
                    numLinea ++;
                    return COMMENT; // Paramos cuando encontramos un salto de linea
                } else {
                    //Si no se trata de un'/'o  '*' , entonces se trata del operador '/'
                    undoNextCharacter();
                    undoNextCharacter();
                    return automatonOperators(); // Redireccionamos al autómata de operadores
                }
                break;

            case GENERAL_COMMENT:
                // En caso de identificar el incio de un comentario multilinea, continuamos leyendo hasta
                // encontrar el caracter '*', que marcará el incio del cierre de comentario
                while (character != '*') {
                    if (character == '\n') {
                        numLinea ++;
                        flagLineFeed = 1;
                    }
                    character = nextCharacter();
                }
                stateComment = GENERAL_COMMENT_END;
                break;

            case GENERAL_COMMENT_END:
                // Cuando identificamos el incio del cierre de comentario, solo confirmamos dicho cierre si
                // este va seguido del caracter '/'. En otro caso, aun no se ha inciado el cierre.
                if (character == '/') {
                    if (flagLineFeed) {
                        // Cuando un comentario multilinea incluye un salto de linea, se considera tod0
                        // el comentario como un salto de linea en si mismo.
                        numLinea ++;
                        restartLexemaBuffer();
                        strcpy(lexemaBuffer, "\\n");
                        return LINE_FEED;
                    } else {
                        return COMMENT;
                    }
                } else {
                    stateComment = GENERAL_COMMENT;
                }
                break;

        }
        getNextCharacter();
    }
}

// Autómata encargado de identificar los paréntesis
int automatonBrackets() {
    // Recorremos el array de paréntesis
    int i = 0;
    while (bracketsArray[i] != '\0') {
        if (character == bracketsArray[i]) {
            return bracketsArray[i];
        }
        i ++;
    }
    return NOT_FINAL_STATE;
}

// Autómata encargado de identificar las cádenas de caractéres
int automatonStrings() {
    int state = INITIAL_STATE;
    int i = 0;

    while (1) {
        switch (state) {
            case INITIAL_STATE:
                // Debemos distinguir entre las cadenas que comienzan por '"' o '`', para
                // verificar que se cierran correctamente
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
                // Continuamos leyendo caracteres hasta que encontremos el '"' de cierre de cadena
                while (character != '"') {
                    character = nextCharacter();
                    if (character == EOF || character == '\n') {
                        // Si encontarmos un salto de linea o llegamos al final del código fuente,
                        // se trata de una cadena sin cerrar
                        showError(NOT_CLOSED_STRING, numLinea);
                        return EOF;
                    } else if (character == '\\') {
                        // En caso de encontrar un '\', tenemos que verificar que se esté escapando un
                        // caracter válido.
                        getNextCharacter();
                        while (escapedChar[i] != '\0') {
                            if (character == escapedChar[i]) {
                                character = nextCharacter();
                                break;
                            }
                            i ++;
                        }
                        if (escapedChar[i] == '\0') {
                            showError(ILLEGAL_ESCAPED_CHARACTER, numLinea);
                        }
                    }
                }
                return STRING_LIT;

            case START_OF_RAW_STRING:
                getNextCharacter();
                // Continuamos leyendo caracteres hasta que encontremos el '`' de cierre de cadena
                while (character != '`') {
                    character = nextCharacter();
                    if (character == EOF) {
                        // Si llegamos al final del código fuente,
                        // se trata de una cadena sin cerrar
                        showError(NOT_CLOSED_STRING, numLinea);
                        return EOF;
                    } else if (character == '\\') {
                        // En caso de encontrar un '\', tenemos que verificar que se esté escapando un
                        // caracter válido.
                        getNextCharacter();
                        while (escapedChar[i] != '\0') {
                            if (character == escapedChar[i]) {
                                character = nextCharacter();
                                break;
                            }
                            i ++;
                        }
                        showError(ILLEGAL_ESCAPED_CHARACTER, numLinea);
                    } else if (character == '\n') {
                        // Los saltos de linea están permitidos, aumentamos el numero de lineas leidas del c.fuente
                        numLinea ++;
                    }
                }
                return STRING_LIT;
        }
    }
}

// Autómata encargado de reconocer identificadores, es decir, cadenas alfanumericas
int automatonIds(){
    while (1) {
        getNextCharacter();
        if (! isalpha(character) && ! isdigit(character) && character != '_') {
            return IDENTIFIER;
        }
    }
}


lexemaOutput nextLexicalComponent() {
    character = ' ';            // Reiniciamos el caracter actual
    restartLexemaBuffer();      // Reiniciamos el buffer en el que se construye el lexema

    while (1) {
        /* Se pide un nuevo caracter al sistema de entrada, hasta que se reconoce un nuevo
         * lexema, momento en el cual se sale de este blucle
         */
        getNextCharacter();
        if (character == EOF) {
            //Si se lee el EOF, hemos llegado al final del código fuente.
            return lexemaFinded(EOF, false);
        }
        else if (! isalpha(character) && ! isdigit(character) && character != '_') {
            /* En este bloque identificamos cualquier cosa que no sea una letre ('_' incluido)
             * o un digito. En caso de que el caracter sea reconocido por un automáta, el flag
             * finded pasará a tener el valor del componente lexico correspondiente. En caso
             * contrario mantendrá el estado NON_FINAL_STATE
             */
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
            if (finded == COMMENT) {
                // Ignoramos los comentarios, y no los devolvemos como un lexema, pues no sno de interés,
                restartLexemaBuffer();
                break;
            }
            if (finded == NOT_FINAL_STATE) {
                // Si el caracter no fue reconocido por ninguno de los automátas, entonces
                // verificamos si se trata de un espacio o una tabulacion, caracteres que ignoramos.
                // Si no se trata de ninguno de estos caracteres especiales, entonces hemos leido
                // un caracter ilegal para nuestro lenguaje.
                if((character == ' ' || character =='\t'))
                    restartLexemaBuffer();
                else{
                    showError(ILLEGAL_CHARACTER, numLinea);
                    restartLexemaBuffer();
                }
                break;
            }

            return lexemaFinded(finded, false);
        } else if (isdigit(character)) {
            // Si el caracter es un dígito, entonces redirigimos al automáta encargado de identificar números
            // En caso de reconocer un lexema, lo devolvemos asociandolo con el componente léxico identificado.
            return lexemaFinded(automatonNumbers(), false);
        } else if (isalpha(character) || character == '_') {
            // Si el caracter es un dígito, entonces redirigimos al automáta encargado de reconocer identificadores
            // En caso de reconocer un lexema, lo devolvemos asociandolo con el componente léxico identificado.
            return lexemaFinded(automatonIds(), true);
        }
    }
    return lexemaFinded(NOT_FINAL_STATE, false);
}


