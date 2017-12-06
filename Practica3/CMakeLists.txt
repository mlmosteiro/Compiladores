cmake_minimum_required(VERSION 3.8)
project(Practica3)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99  -Wall -Wextra -pedantic-errors")
set(SOURCE_FILES main.c Definitions.h Definitions.c SymbolsTable/SymbolsTable.c SymbolsTable/SymbolsTable.h ErrorManager.c ErrorManager.h
        SymbolsTable/BST.c SymbolsTable/BST.h lex.yy.c lex.yy.h consolaMatematica.tab.c consolaMatematica.tab.h)
add_executable(Practica3 ${SOURCE_FILES})

target_link_libraries(Practica3 m)