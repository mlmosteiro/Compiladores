%{
    #include <stdlib.h> 
    #include <stdio.h>  
    #include <math.h>   

    extern int yylex(void);
    extern void yyterminate();
    void yyerror(const char *s);
    extern FILE* yyin;
    void man();
    void printByType(int type);
%}

%code top{
  void yyerror (char const *s);
}

%code requires {
    #include "SymbolsTable/SymbolsTable.h"
    #include "Definitions.h"
    #include "ErrorManager.h"
}

%define api.value.type union /* Generate YYSTYPE from these types:  */
%token <int>                END_OF_FILE
%token <int>                RESTART_WORKSPACE
%token <int>                LOAD_DEFAULT_CONSTANTS
%token <int>                LOAD_DEFAULT_FUNCTIONS
%token <int>                SHOW_CONSTANTS
%token <int>                SHOW_FUNCTIONS
%token <int>                SHOW_VARIABLES
%token <int>                SHOW_SYMBOL_TABLE
%token <int>                HELP
%token <int>                QUIT
%token <double>             NUM         
%token <symbolInput*>       VAR FNCT CONS
%type  <double>             exp

%precedence                 '='
%left                       '-' '+'
%left                       '*' '/'
%precedence                 NEG
%right                      '^'

%%

input:      
            | input line
;

line:       '\n'
            | END_OF_FILE
            | QUIT '\n'                 { YYACCEPT;}
            | HELP '\n'                 { man();}
            | SHOW_VARIABLES '\n'       { printByType(VAR);}
            | SHOW_FUNCTIONS '\n'       { printByType(FNCT);}
            | SHOW_CONSTANTS '\n'       { printByType(CONS);}
            | SHOW_SYMBOL_TABLE '\n'    { printByType(0);}
            | RESTART_WORKSPACE '\n'    { // Libera la tabla de simbolos actual, y crea una nueva
                                          destroySymbolsTable();
                                          initSymbolsTable();
                                        }
            | LOAD_DEFAULT_FUNCTIONS '\n'   {loadFunctions(functions);}
            | LOAD_DEFAULT_CONSTANTS '\n'   {loadConstants(constants);};
            | exp ';' '\n'              { ; }
            | exp '\n'                  { printf ("%.10g\n", $1); }
            | error '\n'                { yyerrok; }
;

exp:        NUM         { $$ = $1;}
            | VAR   {
                        if($1->type == FNCT){
                            $$ = 0;
                            showError(VALUE_OF_FUNCTION,-1);
                        }else{
                            if(!$1->initialized)
                            showError(NOT_INITIALIZED_VARIABLE,-1);
                            $$ = $1->value.var;
                        }
                    }
            
            | VAR '=' exp {
                        if($1->type == VAR){
                            $$ = $3; $1->value.var = $3;
                            $1->initialized = true;
                        }else{
                            showError(OVERWITE_VARIABLE,-1);
                            $$ = 0;
                        }
                    }
            
            | VAR '(' exp ')'{
                        if($1->type == FNCT)
                            $$ = (*($1->value.fnctptr))($3);
                        else{
                            $$ = 0;
                            showError(NOT_FUNCTION,-1);
                        }
                      }
            
            | exp '+' exp       { $$ = $1 + $3;}
            | exp '-' exp       { $$ = $1 - $3;}
            | exp '*' exp       { $$ = $1 * $3;}
            | exp '/' exp       { if($3 == 0) //error if user divides by zero
                                    showError(DIVISION_BY_ZERO,-1);
                                  else
                                    $$ = $1 / $3;
                                }
            | '-' exp  %prec NEG { $$ = -$2;}
            | exp '^' exp       { $$ = pow ($1, $3);}
            | '(' exp ')'       { $$ = $2;}
;

%%

void yyerror (char const *s){
  printf("\033[0;31m");
  printf("%s",s);
  printf("\033[0m\n");
}

void man() {
    printf("\n ConsolaMatematicaV1.0 - Ayuda\n");            
    printf("*********************************\n");
    printf("Todos los comandos deben ser precedidos por \":\"");
    printf("\n\t:? --> Muestra esta ayuda");
    printf("\n\t:sf --> Muestra las funciones disponibles (show functions)");
    printf("\n\t:sv --> Muestra las variables actuales (show variables)");
    printf("\n\t:st --> Muestra la tabla de simbolos actual, completa.(Show symbol table)");
    printf("\n\t:load <nombreArchivo> --> Carga un script de comandos para ejecutar");
    printf("\n\t:res --> Reinicia el espacio de trabajo actual (Reset workspace");
    printf("\n\t:ldf --> Carga las funciones por defecto (sin, cos, exp, ...)");
    printf("\n\t:ldf --> Carga las constantes por defecto (e,pi)");
    printf("\n\t:q --> Finaliza el programa (Quit)");
    printf("\n");
}

void printByType(int type){
    switch(type){
        case VAR:
            printf("*********************************\n");
            printf("VARIABLES\n");
            printSymbolByType(VAR);
            break;

        case FNCT:
            printf("*********************************\n");
            printf("FUNCIONES\n");
            printSymbolByType(FNCT);
            break;

        case CONS:
            printf("*********************************\n");
            printf("CONSTANTES\n");
            printSymbolByType(CONS);
            break;

        case 0:
            printf("TABLA DE SIMBOLOS\n");
            printf("*********************************\n");
            printSymbolsTable();
            break;
    }
}