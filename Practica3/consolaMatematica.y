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
    #include "ErrorManager.h"
}

%define api.value.type union /* Generate YYSTYPE from these types:  */
%token <int>                END_OF_FILE
%token <int>                RESTART_WORKSPACE
%token <int>                SHOW_CONSTANTS
%token <int>                SHOW_FUNCTIONS
%token <int>                SHOW_VARIABLES
%token <int>                SHOW_SYMBOL_TABLE
%token <int>                HELP
%token <int>                QUIT
%token <double>             NUM         
%token <symbolInput*>       VAR FNCT    
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
            | SHOW_CONSTANTS '\n'       { printByType(-1);}
            | SHOW_SYMBOL_TABLE '\n'    { printByType(0);}
            | RESTART_WORKSPACE '\n'    {   // Libera la tabla de simbolos actual, y crea una nueva
                                            destroySymbolsTable();
                                            initSymbolsTable();
                                        }
            | exp ';' '\n'              { ; }
            | exp '\n'                  { printf ("%.10g\n", $1); }
            | error '\n'                { yyerrok; }
;

exp:        NUM       { $$ = $1; /* gets thje value of a number */ }
            | VAR                {  //access to a var value
                        //checks if users tries to check the value of a function
                        if($1->type == FNCT){
                          $$ = 0;
                          showError(VALUE_OF_FUNCTION,-1);
                       }else{
                         //gets the value of the variable if it's initialized
                         if(!$1->initialized)
                          showError(NOT_INITIALIZED_VARIABLE,-1);
                         $$ = $1->value.var;
                       }
                      }
            | VAR '=' exp        { //if user is accesing a variable, initialices the value
                      if($1->type == VAR){
                        $$ = $3; $1->value.var = $3;
                        $1->initialized = true;
                      }else{
                        showError(OVERWITE_VARIABLE,-1);
                        $$ = 0;
                      }
                     }
            | VAR '(' exp ')'    {  //invokes a function using a expr as an argument
                        if($1->type == FNCT)
                          $$ = (*($1->value.fnctptr))($3);
                        else{
                          $$ = 0;
                          showError(NOT_FUNCTION,-1);
                        }
                      }
                      //basic arithmetic operations
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
    printf("\n CLIMath v0.1 System Help");
    printf("\n===============================");
    printf("\nWelcome to the first version of this command line interface");
    printf("\n");
    printf("\nYou can use \";\" at the end of a command to supress the command output");
    printf("\n");
    printf("\nAvailable options:");
    printf("\n:? --> Shows help menu.");
    printf("\n:f --> Shows availble functions.");
    printf("\n:v --> Shows variables.");
    printf("\n:t --> Shows full symbol table.");
    printf("\n:l --> Load scritp.");
    printf("\n\t :l pathToFile");
    printf("\n:r --> Resets the current workspace.");
    printf("\n:q --> Quit.");
    printf("\n");
}

void printByType(int type){
    switch(type){
        case VAR:
            printf("*********************************\n");
            printf("VARIABLES\n");
            searchLexicalCompAndPrintSymbolsTable(VAR);
            break;

        case FNCT:
            printf("*********************************\n");
            printf("FUNCTIONS\n");
            searchLexicalCompAndPrintSymbolsTable(FNCT);
            break;

        case -1:
            printf("*********************************\n");
            printf("CONSTANTS\n");
            searchLexicalCompAndPrintSymbolsTable(FNCT);
            break;

        case 0:
            printf("Symbol Table\n");
            printf("*********************************\n");
            printSymbolsTable();
            break;
    }
}