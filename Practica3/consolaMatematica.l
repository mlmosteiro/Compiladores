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
    int loadFile(char* filepath);
    int eofAction();
%}

IDENTIFIER                  [[:alpha:]_][[:alnum:]_]*
DECIMAL                     [[:digit:]][[:digit:]]*
INTEGER                     [1-9]({DECIMAL})?|0
EXPONENT                    [Ee][+-]?{DECIMAL}
FLOAT                       ({DECIMAL}\.({DECIMAL})?({EXPONENT})?)|({DECIMAL}{EXPONENT})|(\.{DECIMAL}({EXPONENT})?)


SHOW_VARIABLES              \:sv
SHOW_FUNCTIONS              \:sf
SHOW_CONSTANTS              \:sc
RESTART_WORKSPACE           \:res
LOAD_DEFAULT_CONSTANTS      \:ldc
LOAD_DEFAULT_FUNCTIONS      \:ldf
SHOW_SYMBOL_TABLE           \:st
LOAD                        \:load[ ][^\0\n]+
HELP                        \:\?
QUIT                        \:q

%%

<<EOF>>                     {return eofAction();}

{SHOW_VARIABLES}            return SHOW_VARIABLES;

{SHOW_FUNCTIONS}            return SHOW_FUNCTIONS;

{SHOW_CONSTANTS}            return SHOW_CONSTANTS;

{RESTART_WORKSPACE}         return RESTART_WORKSPACE;

{SHOW_SYMBOL_TABLE}         return SHOW_SYMBOL_TABLE;

{LOAD_DEFAULT_CONSTANTS}    return LOAD_DEFAULT_CONSTANTS;
{LOAD_DEFAULT_FUNCTIONS}    return LOAD_DEFAULT_FUNCTIONS;
{HELP}                      return HELP;

{QUIT}                      {fclose(yyin); return QUIT;}

{LOAD}                      {return loadFile(&yytext[6]);}

{INTEGER}                   {yylval.NUM = atof(yytext); return NUM;}

{FLOAT}						{yylval.NUM = atof(yytext); return NUM;}

{IDENTIFIER}			    {return lexemaFinded();}

[ \t\r]						;
[\n]						{numLinea++; return '\n';}
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

int loadFile(char* filepath){

    buffer = YY_CURRENT_BUFFER;             // Puntero al buffer actual de flex
    FILE* file= fopen(filepath,"r");      // Abrimos el archivo

    if(file != NULL){
        // Creamos un nuevo buffer a partir del archivo y cambiamos el actual por el nuevo
        yyin = file;
        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
        BEGIN(INITIAL);
    }else{
        showError(NOT_FILE_FOUNDED,-1);
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

int eofAction(){
    /* En caso de encontar el final de fichero, cambiamos el buffer por el
     * que habia antes de empezar a leer el fichero, y que tenemos guardado
     * en la variable buffer
    */

    yy_delete_buffer( YY_CURRENT_BUFFER );
    yy_switch_to_buffer(buffer);
    yyin = stdin;
    yyrestart(yyin);
    return END_OF_FILE;
}