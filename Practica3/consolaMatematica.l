%option noyywrap
%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <ctype.h>
    #include "consolaMatematica.tab.h"
    #include "SymbolsTable/SymbolsTable.h"

    int numLinea=1;
    YY_BUFFER_STATE buffer;

    int checkSymbols();
    int lexemaFinded();
    int loadFile();
%}

IDENTIFIER                  [[:alpha:]_][[:alnum:]_]*
DECIMAL                     [[:digit:]][[:digit:]]*
INTEGER                     [1-9]({DECIMAL})?
EXPONENT                    [Ee][+-]?{DECIMAL}
FLOAT                       ({DECIMAL}\.({DECIMAL})?({EXPONENT})?)|({DECIMAL}{EXPONENT})|(\.{DECIMAL}({EXPONENT})?)


SHOW_VARIABLES              \:sv
SHOW_FUNCTIONS              \:sf
SHOW_CONSTANTS              \:sc
RESTART_WORKSPACE           \:res
SHOW_SYMBOL_TABLE           \:st
LOAD                        \:load[ ][^\0\n]+
HELP                        \:\?
QUIT                        \:q

%%

<<EOF>>                     { /*if lexical analyzer founds and EOF sets the yyin to the previous yyin buffer (stdin)*/
                                yy_delete_buffer( YY_CURRENT_BUFFER );
                                yy_switch_to_buffer(buffer);
                                return END_OF_FILE;
                            }

{SHOW_VARIABLES}            return SHOW_VARIABLES;

{SHOW_FUNCTIONS}            return SHOW_FUNCTIONS;

{SHOW_CONSTANTS}            return SHOW_CONSTANTS;

{RESTART_WORKSPACE}         return RESTART_WORKSPACE;

{SHOW_SYMBOL_TABLE}         return SHOW_SYMBOL_TABLE;

{HELP}                      return HELP;

{QUIT}                      return QUIT;

{LOAD}                      {loadFile();}

{INTEGER}                   {yylval.NUM = atof(yytext); return NUM;}

{FLOAT}						{yylval.NUM = atof(yytext); return NUM;}

{IDENTIFIER}			    {return lexemaFinded();}

[ \t\r]						;
[\n]						{numLinea++; return yytext[0];}
[[:punct:]]                 {return checkSymbols();}
.                           

%%



int lexemaFinded() {
    /*
    * Funcion que nos devuelve el componente lexico de una cadena alfanumerica detectada (ID o palabra reservada).
    * En caso de ya existir, entonces estara en la tabla de simbolos y se recuperará de ahi. De lo contrario, 
    * se introduce en la tabla de símbolos.
    */

    symbolInput* isInSymbolTable = search(yytext);
    if (isInSymbolTable == NULL) {
        isInSymbolTable = insert(yytext, VAR);
        isInSymbolTable->initialized = false;
    }

    //sets the yybal to the register of the variable
    *((symbolInput**) &yylval) =  isInSymbolTable;

    return VAR;
}

int loadFile(){
    buffer = YY_CURRENT_BUFFER; ///*Pointer to the current flex buffer*/
    FILE* file= fopen(&yytext[3],"r"); /*Tries to open a file*/

    if(file != NULL){ ///*If file is open*/
        yyin = file; /*creates a new buffer and switchs it with the current buffer*/
        yy_switch_to_buffer(yy_create_buffer( yyin, YY_BUF_SIZE ) );
        BEGIN(INITIAL);
    }else{
      showError(NOT_FILE_FOUNDED,numLinea);
    }

    return '\n';
}

int checkSymbols(){
    if (strcmp("+",yytext) == 0 ) return('+');
    if (strcmp("(",yytext) == 0 ) return('(');
    if (strcmp(")",yytext) == 0 ) return(')');
    if (strcmp("-",yytext) == 0 ) return('-');
    if (strcmp("[",yytext) == 0 ) return('[');
    if (strcmp("]",yytext) == 0 ) return(']');
    if (strcmp("*",yytext) == 0 ) return('*');
    if (strcmp("^",yytext) == 0 ) return('^');
    if (strcmp("{",yytext) == 0 ) return('{');
    if (strcmp("}",yytext) == 0 ) return('}');
    if (strcmp("/",yytext) == 0 ) return('/');
    if (strcmp("=",yytext) == 0 ) return('=');
    if (strcmp(",",yytext) == 0 ) return(',');
    if (strcmp(";",yytext) == 0 ) return(';');
    if (strcmp("!",yytext) == 0 ) return('!');
    if (strcmp("?",yytext) == 0 ) return('?');
    if (strcmp(".",yytext) == 0 ) return('.');
    return '\n';
}
