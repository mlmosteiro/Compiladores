cmake_minimum_required(VERSION 3.8)
project(Practica1)

set(CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99  -Wall -Wextra -pedantic-errors")
set(SOURCE_FILES main.c InputSystem.c InputSystem.h Definitions.h LexicalAnalyzer.c LexicalAnalyzer.h SintacticAnalyzer.c SintacticAnalyzer.h SymbolsTable/SymbolsTable.c SymbolsTable/SymbolsTable.h ErrorManager.c ErrorManager.h SymbolsTable/BST.c SymbolsTable/BST.h)
add_executable(Practica1 ${SOURCE_FILES})