%option noyywrap 
%{
	#include <string.h>
	#include <stdio.h>
	#include <ctype.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include "LexicalAnalyzer.h"
	#include "InputSystem.h"
	#include "SymbolsTable/SymbolsTable.h"
	#include "Definitions.h"
	#include "ErrorManager.h"

	/* Arrays en los que establecemos los parenresis, operadores, separadores o caracteres que se pueden escapar*/
	char bracketsArray[7]={'(',')','[',']','{','}','\0'};
	char operatorsArray[7] = {'-', '*', '^', '!', '%', '|', '\0'};
	char separatorsArray[4] = {',', ';', '\n', '\0'}; // Tambien podriamos usar strpbrk(), pero no lo haremos.
	char escapedChar[11] = {'n', 'a', 'b', 'f', 'r', 't', 'v', '\\', '\"', '\'', '\0'};
%}

LETTER 				[_a-zA-Z]
DECIMAL_DIGIT		[0-9]
OCTAL_DIGIT 		[0-7]
HEX_DIGIT 			[0-9a-fA-F]
IDENTIFIER 			{LETTER}({LETTER}{DECIMAL_DIGIT})*
DECIMAL_LIT 		[1-9]{DECIMAL_DIGIT}*
OCTAL_LIT 			0{OCTAL_DIGIT}*
HEX_LIT 			0(x|X){HEX_DIGIT}{HEX_DIGIT}*
INTEGER_LIT 		{DECIMAL_LIT}{OCTAL_LIT}{HEX_LIT}
DECIMAL  			{DECIMAL_DIGIT}{DECIMAL_DIGIT}*
EXPONENT 			(E|e)[+-]+{DECIMAL}
FLOAT_LIT 			({DECIMAL}"." {DECIMAL}+ {EXPONENT}+) | {DECIMAL}{EXPONENT} | "." {DECIMAL} {EXPONENT}+
IMAGINARY_LIT 		({DECIMAL} | {FLOAT_LIT})"i"
%%
{IDENTIFIER}		{printf("IDENTIFICADOR");}
{HEX_LIT}			{printf("ENTERO");}
{OCTAL_LIT}			{printf("ENTERO");}
{INTEGER_LIT}		{printf("ENTERO");}
{FLOAT_LIT}			{printf("FLOTANTE");}
{IMAGINARY_LIT}		{printf("IMAGINARIO"); printHello();}
%% 
main(){
        printf("HELLO");
}
