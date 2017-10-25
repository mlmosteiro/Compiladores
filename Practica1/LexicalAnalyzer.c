#include "Definitions.h"
#include "InputSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LexicalAnalyzer.h"
#include "SymbolsTable/SymbolsTable.h"

//Todo: gestion de errores

char* lexemaBuffer;
int mult = 1;
char character=' ';
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
    int indexOfChar = -1;
    character=' ';
    restartLexemaBuffer();


    while (character != '\000') {
        //Simulamos que encontramos un lexema
        for(int i = 0; i<6;i++){
            character = nextCharacter();
            if(character == '\000' && indexOfChar == 0){
                return NULL;
            }

            indexOfChar++;
            printf("SA: %c\n", character);
            if(indexOfChar == SIZE_SA){
                extendLexemaBuffer();
            }
            lexemaBuffer[indexOfChar]=character;
        }

        insert(lexemaBuffer, indexOfChar, numLinea);
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

int automatonNumbers() {
    int stateNumber = 0;
    while (1) {
        character = nextCharacter();

        //Encontramos algún símbolo que ya no pertenece al lexema númerico (delimitador)
        //TODO: TENER EN CUENTA LAS LETRAS DEL HEXADECIMAL
        if (! isalpha(character) && ! isdigit(character) && character != '_' && character != '.') {
            // lexemaBuffer[pos - 1] = '\0'; /TODO: Eliminar el último caracter porque con el nos damos cuenta que ya estámos leyendo otra cosa
            previousCharacter();
            switch (stateNumber) {
                //TODO: AÑADIR TODOS LOS NUMEROS POSIBLES

                case 0:
                    return printWhenLexFind(lexemaBuffer, NUMBER_INTEGER_LIT);
                case 1:
                    return printWhenLexFind(lexemaBuffer, NUMBER_DECIMAL_LIT);
                case 2:
                    return printWhenLexFind(lexemaBuffer, NUMBER_OCTAL_LIT);//TODO CAMBIARLO PARA QUE FUNCIONE CON OCTAL
                case 4:
                    return printWhenLexFind(lexemaBuffer, NUMBER_HEX_LIT);//TODO CAMBIARLO PARA QUE FUNCIONE CON HEXADECIMA
            }
        }

        switch (stateNumber) {
            case 0: // Enteros : decimal,
                if (character != '0' && (isdigit(character) || character == '_')) {
                    stateNumber = NUMBER;
                } else if (character == '0') {
                    stateNumber = NUMBER_NOT_DECIMAL;
                } else {
                    // TODO: Gestor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_NUMBER, line, lexemaBuffer);
                    return nextLexicalComponent();
                }
                break;
            case NUMBER_NOT_DECIMAL:
                if (character == '.') {
                    stateNumber = NUMBER_FLOAT_LIT;
                } else if (isdigit(character) && character <= '7') {
                    stateNumber = NUMBER_OCTAL_LIT;
                } else if (character == 'x') {
                    stateNumber = NUMBER_HEX_LIT;
                } else if (character == 'i') {
                    stateNumber = NUMBER_IMAGINARY_LIT;
                } else {
                    // TODO: Gestor de errores : error con formato erroneo
                }
                break;

            case NUMBER_FLOAT_LIT: //DOUBLE
                if (isdigit(character) || character == '_' || character == '.') {
                    stateNumber = NUMBER_FLOAT_LIT;
                } else if (character == 'e' || character == 'E') {
                    stateNumber = NUMBER_EXPONENTIAL_LIT;
                } else {
                    // TODO: GEstor de errores : error con formato erroneo
                    //printError(BAD_FORMATED_DOUBLE, line, lexemaBuffer);
                    return nextLexicalComponent();
                }
                break;
            case NUMBER_EXPONENTIAL_LIT://TODO: FIJAR QUE EL NUMERO NO PUEDE ACABAR SOLO EN E
                if (character == '+' || character == '-' || character == '_' || (isdigit(character))) {
                    stateNumber = NUMBER_EXPONENTIAL_LIT;
                } else {
                    // TODO: GEstor de errores : error con formato erroneo
                    // printError(BAD_FORMATED_EXP, line, lexemaBuffer);
                    return nextLexicalComponent();
                }
                break;
            case NUMBER_HEX_LIT:
                if (isdigit(character) || character < 'G' || character < 'g' || character >= 'A' || character >= 'a') {
                    NUMBER_HEX_LIT;
                } else {
                    //TODO : GESTOR DE ERRORES
                }
                break;
            case NUMBER_IMAGINARY_LIT:
                break;
        }
    }
}

//TODO: INFO: PARAMOS EL AUTOMATA CUADNO LEEMOS UN CARACTER QUE YA no ppertenece a ese lexema.
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

