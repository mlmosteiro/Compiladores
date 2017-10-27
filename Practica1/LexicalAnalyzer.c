#include "Definitions.h"
#include "InputSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"

#define INITIAL_STATE 900
#define NUMBER_NOT_DECIMAL_LIT 901
#define NUMBER_EXP_SIG 902

//Todo: gestion de errores

char* lexemaBuffer;
int mult = 1;
char character=' ';
int currentBufferIndex = -1;
int numLinea=0;
char operatorsArray[7] = {'+','-','*','/','<','>','\0'}; // Todo: modificar mayor o igual
char separatorsArray[7] = {',',';',' ','.','\n','\0'};

void initLexicalAnalyzer() {
    initInputSystem("/home/maryluz/CLionProjects/Compiladores/Practica1/e.txt");
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

void extendLexemaBuffer() {
    lexemaBuffer = (char *) realloc(lexemaBuffer, ((mult++) * sizeof(char) * SIZE_SA));
}

void restartLexemaBuffer() {
    free(lexemaBuffer);
    lexemaBuffer = (char *) malloc(sizeof(char) * SIZE_SA);
}

char * nextLexicalComponent() {

    character=' ';
    restartLexemaBuffer();


    while (character != '\000') {
        //Simulamos que encontramos un lexema
        for(int i = 0; i<6;i++){
            character = nextCharacter();
            if(character == '\000' && currentBufferIndex == 0){
                return NULL;
            }

            currentBufferIndex++;
            printf("SA: %c\n", character);
            if(currentBufferIndex == SIZE_SA){
                extendLexemaBuffer();
            }
            lexemaBuffer[currentBufferIndex]=character;
        }

        insert(lexemaBuffer, currentBufferIndex, numLinea);
        return lexemaBuffer;
    }
}

void destroyLexicalAnalyzer(){
    free(lexemaBuffer);
}

int printWhenLexFind(char * lexema, int lexicalComp){
    printf("< %s > - < %d >\n", lexema, lexicalComp);
    return lexicalComp;
}

/*
 * Este método se encarga de reconocer
 * { (,),[,],{,} }
 */

int automatonBrackets() {
    if (character == '(') {
        return printWhenLexFind(lexemaBuffer, '(');
    } else if (character == ')') {
        return printWhenLexFind(lexemaBuffer, ')');
    } else if (character == '[') {
        return printWhenLexFind(lexemaBuffer, '[');
    } else if (character == ']') {
        return printWhenLexFind(lexemaBuffer, ']');
    } else if (character == '{') {
        return printWhenLexFind(lexemaBuffer, '{');
    } else if (character == '}') {
        return printWhenLexFind(lexemaBuffer, '}');
    }
    return - 1;
}

int automatonOperators() {
    int i = 0;
    while (operatorsArray[i] != '\0') {
        if (character == operatorsArray[i]) {
            return printWhenLexFind(lexemaBuffer, operatorsArray[i]);
        }
        i ++;
    }

    if (character == '=') {
        character = nextCharacter();
        //TODO: Aqui tener cuidado con la diferencia entre := e ==
        if (character != '=') {
            character = previousCharacter();
            return printWhenLexFind(lexemaBuffer, ASSIGNATION);
        }
        if (character == '=') {
            return printWhenLexFind(lexemaBuffer, EQUALS);
        }
    }
    return - 1;
}

int automatonSeparators(){
    int i = 0;
    while(separatorsArray[i]!='\0'){
        if(character==separatorsArray[i]){
            if(character!='\n' && character!=' '){
                return printWhenLexFind(lexemaBuffer, separatorsArray[i]);
            }
            if(character=='\n'){
                numLinea++;
            }
            if(character=='\n' | character==' ') {
                return nextLexicalComponent(); //TODO: CAMBIAR ESTO
            }
        }
        i++;
    }
    return -1;
}

int automatonStrings(){
    int stateComment = 0;
    while(1){
        switch(stateComment) {
            case 0:
                if (character == '"') {
                    stateComment = 1;
                } else {
                    return - 1;
                }
                break;
            case 1:
                if (character == '"') {
                    printWhenLexFind(lexemaBuffer, STRING);
                    return STRING;
                } else if (character == 92) { // Caracter \ --- TODO: MIRAR LA EXISTENCIA DE ESTE CARACTER EN GO
                    nextCharacter();
                } else if (character == '\n') {
                    numLinea ++;
                    //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                    return - 1;
                } else if (character == END_OF_FILE) {
                    //printf("Error: [ %s ] LC [ %d ] \n", "String no cerrada \n", STRING);
                    //TODO: GESTOR DE ERRORES, LA CADENA NO ACABA
                    return END_OF_FILE;
                }
                break;

        }
        character = nextCharacter();
    }
}

int lexemaFinded(int componenteLexico){
    lexemaBuffer[currentBufferIndex] = '\0';
    previousCharacter();
    symbolImput* isInSymbolTable = search(lexemaBuffer);
    if(isInSymbolTable == NULL){
        insert(lexemaBuffer, componenteLexico, numLinea);
    }
    return componenteLexico;
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
                } else if(character == '.') {                                       // - es un '.'
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else{
                    // TODO: Gestor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_NUMBER, line, lexemaBuffer);          // hemos leido algo que NO forma parte de un número
                }
                break;

            case NUMBER_INTEGER_LIT:                                                // Habiendo leido por lo menos un digito decimal, y a continuacion leemos:
                if(isdigit(character)){                                             // - un digito decimal
                    stateNumber=NUMBER_INTEGER_LIT;                                 // podemos decir que, por ahora, se sigue tratando de un número decimal
                } else if(character == '.') {                                       // - un '.',
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // - una 'e'
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que, por ahora, se trata de un número con exponente (interesante para verificar formato correcto)
                } else if (character == 'i'){                                       // - una 'i'
                     stateNumber = NUMBER_IMAGINARY_LIT;                            // podemos decir que se ha leido un número imaginario
                } else{
                    // TODO: Gestor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_NUMBER, line, lexemaBuffer);          // hemos leido algo que NO forma parte de un número, ya es el siguiente lexema
                    return lexemaFinded(NUMBER_INTEGER_LIT);
                }
                break;

            case NUMBER_NOT_DECIMAL_LIT:                                            // Habiendo leido por lo menos un digito entero no decimal, y acontinuación leemos:
                if (character == '.'){                                              // - un '.'
                    stateNumber = NUMBER_FLOAT_LIT;                                 // podemos decir que, por ahora, se trata de un número en punto flotante
                } else if (isdigit(character) && (character >= '0' || character< '8')) { // - un dígito octal
                    stateNumber = NUMBER_OCTAL_LIT;                                 // podemos decir que, por ahora, se trata de un número octal
                } else if (character == 'x' || character == 'X') {                  // - una 'x'
                    stateNumber = NUMBER_HEX_LIT;                                   // podemos decir que, por ahora, se trata de un número hexadecimal
                } else if (character == 'i') {                                      // - una 'i'
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    return lexemaFinded(NUMBER_INTEGER_LIT);                         // Ya estamos leyendo otro lexema, lo que significa que hemos leido el digito '0' decimal
                }
                break;

            case NUMBER_OCTAL_LIT:                                                  // Si estamos en este estado, estamos seguros de que seguiremos leyendo un
                if (isdigit(character) && (character >= '0' || character < '8')){   // numero octal
                    stateNumber = NUMBER_OCTAL_LIT;
                } else{                                                             // Si no se lee un caracter octal, hemos empezado a leer otro lexema distinto.
                    return lexemaFinded(NUMBER_INTEGER_LIT);
                }
                break;

            case NUMBER_HEX_LIT:
                if (isdigit(character)  || character >= 'A' || character >= 'a'     // Si estamos en este estado, estamos seguros de que seguiremos leyendo
                                        || character <= 'F' || character <= 'f') {  // un numero hexadecimal
                    stateNumber = NUMBER_HEX_LIT;
                } else{                                                             // Si no se lee un caracter hexadecimal, hemos empezado a leer otro lexema distinto.
                    return lexemaFinded(NUMBER_INTEGER_LIT);
                }
                break;

            case NUMBER_FLOAT_LIT: // Números en punto flotante
                if (isdigit(character)) {                                           // Si estamos en este estado, es porque hemos leido ya un '.' en algun momento anterior,
                    stateNumber = NUMBER_FLOAT_LIT;                                 // entonces estamos seguros de que seguiremos leyendo un número en punto flotante
                } else if (character == 'e' || character == 'E') {                  // Si leemos una 'e',
                    stateNumber = NUMBER_EXPONENTIAL_LIT;                           // podemos decir que se ha leido un número con exponente  (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {                                                            // En cualquier otro caso, hemos empezado a leer otro lexema distinto
                    // TODO: GEstor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_DOUBLE, line, lexemaBuffer);
                    return lexemaFinded(NUMBER_FLOAT_LIT);
                }
                break;

            case NUMBER_EXPONENTIAL_LIT://TODO: FIJAR QUE EL NUMERO NO PUEDE ACABAR SOLO EN E   // Si estamos en este estado, es porque hemos leido ya una 'e' en algun momento anterior,
                if ((isdigit(character))) {                                         // entonces estamos seguros de que seguiremos leyendo un número con exponente
                    stateNumber = NUMBER_EXPONENTIAL_LIT;
                } else if (character == '+' || character == '-'){                   // Si leemos un signo '+' o '-' entonces hemos leido
                    stateNumber = NUMBER_EXP_SIG;                                   // un exponente con signo (interesante para verificar formato correcto)
                } else if (character == 'i') {                                      // Si leemos una 'i',
                    stateNumber = NUMBER_IMAGINARY_LIT;                             // podemos decir que se ha leido un número imaginario
                } else {
                    // TODO: GEstor de errores : error con formato erroneo
                    // printError(BAD_FORMATED_EXP, line, lexemaBuffer);            // En cualquier otro caso, el formato ES ERRONEO.
                    return lexemaFinded(NUMBER_FLOAT_LIT);
                }
                break;
            case NUMBER_EXP_SIG:                                                    // Si estamos en este estado, es porque hemos leido ya un exponente con signo '+' o '-'
                if (isdigit(character)) {                                           // Ahora solo se seperan digitos decimales
                    stateNumber = NUMBER_EXP_SIG;
                } else{                                                             // En cualquier otro caso, el formato ES ERRONEO

                }
                break;

            case NUMBER_IMAGINARY_LIT:                                              // Cualquier cosa que leamos a continuacion ya es otro lexema
                return lexemaFinded(NUMBER_IMAGINARY_LIT);
        }
    }
}

//TODO: INFO: PARAMOS EL AUTOMATA CUADNO LEEMOS UN CARACTER QUE YA no ppertenece a ese lexema.
/*  //Encontramos algún símbolo que ya no pertenece al lexema númerico (delimitador)
        //TODO: TENER EN CUENTA LAS LETRAS DEL HEXADECIMAL
        if (! isalpha(character) && ! isdigit(character) && character != '_' && character != '.') {
            // lexemaBuffer[pos - 1] = '\0'; /TODO: Eliminar el último caracter porque con el nos damos cuenta que ya estámos leyendo otra cosa
            previousCharacter();
            switch (stateNumber) {
                //TODO: AÑADIR TODOS LOS NUMEROS POSIBLES

                case NUMBER_INTEGER_LIT:
                case NUMBER_OCTAL_LIT:
                case NUMBER_HEX_LIT:
                    return printWhenLexFind(lexemaBuffer, NUMBER_INTEGER_LIT);
                case NUMBER_FLOAT_LIT:
                case NUMBER_EXPONENTIAL_LIT:
                case NUMBER_EXP_SIG:
                    return printWhenLexFind(lexemaBuffer, NUMBER_FLOAT_LIT);
                case NUMBER_IMAGINARY_LIT:
                    return printWhenLexFind(lexemaBuffer, NUMBER_IMAGINARY_LIT);
            }
        }
*/


/*

int nextLexicalComponent() {

    state = 0;
    pos = 0;

    buffer[0] = '\0';

    while (1) {
        nextCharacter();
        switch (state) {
            case BEGIN:
                //END OF FILE
                if (character == END_OF_FILE) {
                    printf("\n END OF FILE -- [ %s ] LC [ %d ] \n", "END", END_OF_FILE);
                    return END_OF_FILE;
                }
                    //This else analyze separators, comments, operators, brackets and strings
                else if (!isalpha(c) && !isdigit(c) && c!='_') {
                    //printf("\n Delimiter |%s| \n", buffer);
                    int flag=0;
                    flag = automatonSeparators();
                    if(flag == -1){
                        flag = automatonComments();
                    }
                    if(flag==-1){
                        flag = automatonOperators();
                    }
                    if(flag==-1){
                        flag = automatonBrackets();
                    }
                    if(flag==-1){
                        flag = automatonStrings();
                    }
                    if(flag ==-1){
                        printError(UNEXPECTED_CHAR,line,buffer);
                        return nextLexicalComponent();
                    }
                    return flag;
                }
                    //START BY DIGIT
                else if (isdigit(c)) {
                    return automatonNumbers();
                }
                    //START BY LETTER
                else if (isalpha(c) || c == '_') {
                    state = LETTER;
                }
                //printf("Nos encontramos en el estado [ 0 ] con buffer [ %s ]\n", buffer);
                break;
            case LETTER: //LETTER (we arrive here only with a previous LETTER read)
                if (!isalpha(c) && !isdigit(c) && c!='_') {
                    buffer[pos - 1] = '\0';
                    previousChar();
                    SymbolEntry *s;
                    s = search(buffer);
                    if(s!=NULL){
                        printWhenLexFind( buffer, s->lexicalComponent);
                        return s->lexicalComponent;
                    }
                    else{
                        s = (SymbolEntry*)malloc(sizeof(SymbolEntry));

                        s->lexema = (char*)malloc(sizeof(char)*sizeLexema(buffer));
                        strcpy(s->lexema,buffer);
                        s->lexicalComponent = IDENTIFIER;
                        insertItem(*s);
                        printf("LEX: [ %s ] LC [ %d ] \n", buffer, IDENTIFIER);
                        free(s->lexema);
                        free(s);
                        return  IDENTIFIER;
                    }
                }
                if (isalpha(c) | c == '_') {
                    state = LETTER;
                }
                break;
            default:
                printError(UNEXPECTED_CHAR,line,buffer);
                return nextLexicalComponent();
        }
    }
}
*/

