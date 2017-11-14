%option noyywrap 
%{
	#include <string.h>
	#include <stdio.h>
	#include <ctype.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include "SymbolsTable/SymbolsTable.h"
	#include "Definitions.h"
	#include "ErrorManager.h"

	int numLinea = 1;

	//Cabeceras de funciones
	int lexemaFinded();
	int checkSymbols();
	int checkComment();
%}

IDENTIFIER 					[[:alpha:]_][[:alnum:]_]*

DECIMAL						[[:digit:]][[:digit:]]*

DECIMAL_LIT 				[1-9]({DECIMAL})?
NUMBER_OCTAL_LIT 			0[0-7]*
NUMBER_HEX_LIT 				0[xX][[:xdigit:]][[:xdigit:]]*
NUMBER_INTEGER_LIT 			(({DECIMAL_LIT})|({NUMBER_OCTAL_LIT})|({NUMBER_HEX_LIT})) 

EXPONENT 					[Ee][+-]?{DECIMAL}
NUMBER_FLOAT_LIT 			({DECIMAL}\.({DECIMAL})?({EXPONENT})?)|({DECIMAL}{EXPONENT})|(\.{DECIMAL}({EXPONENT})?)

NUMBER_IMAGINARY_LIT 		(({DECIMAL})|({NUMBER_FLOAT_LIT}))i

STRING_RAW					\`.*\`
STRING 						\".*\"
STRING_LIT					(({STRING})|(STRING_RAW))

%%

{IDENTIFIER}	    		{return lexemaFinded();}
{NUMBER_INTEGER_LIT}		{return NUMBER_INTEGER_LIT;}		
{NUMBER_FLOAT_LIT}			{return NUMBER_FLOAT_LIT;}			
{NUMBER_IMAGINARY_LIT}		{return NUMBER_IMAGINARY_LIT;}		
\".*\"						{return STRING_LIT;}
"/*"						{++numLinea; checkComment();}
"\/\/".*					;


[[:punct:]]					return checkSymbols();		
\n                  		{yytext = "\\n"; ++numLinea; return LINE_FEED;}
" "              	 		;
\t 							;
"+="						return (PLUS_EQUAL);
"=="						return (EQUALS);
">="						return (GREATER_EQUAL);
"<<"						return (LESS_LESS);
"++"						return (PLUS_PLUS);
":="						return (DECLARATION_ASSIG);
"<="						return (LESS_EQUAL);
"<-"						return (RECEIVE);
">>"						return (GREATER_GREATER);

\".							{showError(NOT_CLOSED_STRING,numLinea); yyterminate();}
.                  		 	{showError(ILLEGAL_CHARACTER,numLinea);}

%%

int lexemaFinded() {
	/*
	* Funcion que nos devuelve el componente lexico de una cadena alfanumerica detectada (ID o palabra reservada).
	* En caso de ya existir, entonces estara en la tabla de simbolos y se recuperará de ahi. De lo contrario, 
	* se introduce en la tabla de símbolos.
	*/

    symbolImput isInSymbolTable = search(yytext);
    if (isInSymbolTable.lexema == NULL) {
        insert(yytext, IDENTIFIER, numLinea);
        return IDENTIFIER;
    } else {
        return isInSymbolTable.componenteLexico;
    }
}

int checkSymbols(){
    if (strcmp("+",yytext) == 0 ) return('+');
	if (strcmp("&",yytext) == 0 ) return('&');
	if (strcmp("(",yytext) == 0 ) return('(');
	if (strcmp(")",yytext) == 0 ) return(')');
	if (strcmp("-",yytext) == 0 ) return('-');
	if (strcmp("|",yytext) == 0 ) return('|');
	if (strcmp("<",yytext) == 0 ) return('<');
	if (strcmp("[",yytext) == 0 ) return('[');
	if (strcmp("]",yytext) == 0 ) return(']');
	if (strcmp("*",yytext) == 0 ) return('*');
	if (strcmp("^",yytext) == 0 ) return('^');
	if (strcmp(">",yytext) == 0 ) return('>');
	if (strcmp("{",yytext) == 0 ) return('{');
	if (strcmp("}",yytext) == 0 ) return('}');
	if (strcmp("/",yytext) == 0 ) return('/');
	if (strcmp("=",yytext) == 0 ) return(ASSIGNATION);
	if (strcmp(",",yytext) == 0 ) return(',');
	if (strcmp(";",yytext) == 0 ) return(';');
	if (strcmp("%",yytext) == 0 ) return('%');
	if (strcmp("!",yytext) == 0 ) return('!');
	if (strcmp(".",yytext) == 0 ) return('.');
	if (strcmp(":",yytext) == 0 ) return(':');
	showError(ILLEGAL_CHARACTER,numLinea);
	return ILLEGAL_CHARACTER;
}

int checkComment(){
	register int c;
	while(1){											// Leemos tod0 el comentario
		c = input();									// se lee el proximo caracter del flujo de entrada
		if ( c == '*' ) {								// puede que se trate del cierre del comentario o del simbolo *
			while ( (c = input()) == '*' );
			if ( c == '/' )								// confirmamos cierre
				break;
		}
		else if (c == '\n'){							// en caso de saltos de linea, reajustamos el contador
			++numLinea;
		}
		else if ( c == EOF ){							// en caso de encontrar el final de fichero, 
			showError(NOT_CLOSED_COMMENT, numLinea);	// se trata de un comentario multilinea NO cerrado
			yyterminate();
		}
	}

}