/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_CONSOLAMATEMATICA_TAB_H_INCLUDED
# define YY_YY_CONSOLAMATEMATICA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 18 "consolaMatematica.y" /* yacc.c:1909  */

    #include "SymbolsTable/SymbolsTable.h"
    #include "Definitions.h"
    #include "ErrorManager.h"

#line 50 "consolaMatematica.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END_OF_FILE = 258,
    RESTART_WORKSPACE = 259,
    LOAD_DEFAULT_CONSTANTS = 260,
    LOAD_DEFAULT_FUNCTIONS = 261,
    SHOW_CONSTANTS = 262,
    SHOW_FUNCTIONS = 263,
    SHOW_VARIABLES = 264,
    SHOW_SYMBOL_TABLE = 265,
    HELP = 266,
    QUIT = 267,
    NUM = 268,
    VAR = 269,
    FNCT = 270,
    CONS = 271,
    NEG = 272
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{

  /* NUM  */
  double NUM;
  /* exp  */
  double exp;
  /* END_OF_FILE  */
  int END_OF_FILE;
  /* RESTART_WORKSPACE  */
  int RESTART_WORKSPACE;
  /* LOAD_DEFAULT_CONSTANTS  */
  int LOAD_DEFAULT_CONSTANTS;
  /* LOAD_DEFAULT_FUNCTIONS  */
  int LOAD_DEFAULT_FUNCTIONS;
  /* SHOW_CONSTANTS  */
  int SHOW_CONSTANTS;
  /* SHOW_FUNCTIONS  */
  int SHOW_FUNCTIONS;
  /* SHOW_VARIABLES  */
  int SHOW_VARIABLES;
  /* SHOW_SYMBOL_TABLE  */
  int SHOW_SYMBOL_TABLE;
  /* HELP  */
  int HELP;
  /* QUIT  */
  int QUIT;
  /* VAR  */
  symbolInput* VAR;
  /* FNCT  */
  symbolInput* FNCT;
  /* CONS  */
  symbolInput* CONS;
#line 111 "consolaMatematica.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CONSOLAMATEMATICA_TAB_H_INCLUDED  */
