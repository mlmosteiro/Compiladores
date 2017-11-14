%option noyywrap 
%{
	#include <string.h>
	#include <stdio.h>
	#include <ctype.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include "LexicalAnalyzer.h"
	#include "SymbolsTable/SymbolsTable.h"
	#include "Definitions.h"
	#include "ErrorManager.h"

	int numLinea = 0;
	/* Arrays en los que establecemos los parenresis, operadores, separadores o caracteres que se pueden escapar*/
	char bracketsArray[7]={'(',')','[',']','{','}','\0'};
	char operatorsArray[7] = {'-', '*', '^', '!', '%', '|', '\0'};
	char separatorsArray[4] = {',', ';', '\n', '\0'}; // Tambien podriamos usar strpbrk(), pero no lo haremos.
	char escapedChar[11] = {'n', 'a', 'b', 'f', 'r', 't', 'v', '\\', '\"', '\'', '\0'};


	//Cabeceras de funciones
	int lexemaFinded();

%}


IDENTIFIER 			[[:alpha:]_][[:alnum:]_]*

DECIMAL				[[:digit:]][[:digit:]]*

DECIMAL_LIT 		[1-9]({DECIMAL})?
OCTAL_LIT 			0[0-7]*
HEX_LIT 			0[xX][[:xdigit:]][[:xdigit:]]*
INTEGER_LIT 		(({DECIMAL_LIT})|({OCTAL_LIT})|({HEX_LIT})) 

EXPONENT 			[Ee][+-]?{DECIMAL}
FLOAT_LIT 			({DECIMAL}\.({DECIMAL})?({EXPONENT})?)|({DECIMAL}{EXPONENT})|(\.{DECIMAL}({EXPONENT})?)

IMAGINARY_LIT 		(({DECIMAL})|({FLOAT_LIT}))i

STRING 				\".*\"
%%
{IDENTIFIER}		{printf("IDENTIFICADOR \n"); lexemaFinded();}
{INTEGER_LIT}		{printf("ENTERO_int\n");}
{FLOAT_LIT}			{printf("float\n");}
{IMAGINARY_LIT}		{printf("IMAGINARY\n");}
{STRING}			{printf("STRING\n");}

\n                  ++numLinea;
" "                 showError(NOT_CLOSED_STRING,numLinea-1);
.                   showError(ILLEGAL_CHARACTER,numLinea-1);


%% 


int lexemaFinded() {
    symbolImput isInSymbolTable = search(yytext);
    if (isInSymbolTable.lexema == NULL) {
        insert(yytext, IDENTIFIER, numLinea);
        return IDENTIFIER;
    } else {
        return isInSymbolTable.componenteLexico;
    }
}



