=Practica 3=

Objetivo:
Construir un analizador léxico con la herramienta FLEX que devuelva los componentes léxicos que aparecen en el programa concurrentSum.go, codificado en el lenguaje Go.

Instrucciones:
	- Para compilar, utilizar el MakeFile. En la consola ejecutar el comando make. Es necesario tener instaladas las librerias de FLEX. Para ello en una maquina ubuntu o similar se puede ejecutar el comando  sudo apt-get install flex
	- Para ejecutar utilizar la siguiente sintaxis
			./practica3 <archivoCodigoFuente> <achivoDefiniciones>

  	Para ejecutar sobre concurrentSum.go:
  			./practica3 concurrentSum.go Definitions.h

	Se podria ejecutar el analizador léxico con cualquier otro conjunto de palabras clave cambiando el archivo Definitions.h, o a otro código fuente.

Para eliminar los archivos generados por la compilacion

    make clean
    make cleanall


# CLIMath

Command Line Interface to execute mathematics operations made as an asingment of the Compilers &amp; Interpreters subject.

Once you are inside the tool, you can type :? to get the system help.

The current version of the tool supports:

- basic arithmetic operations +,-,*,/
- variable declaration
- use of simple math functions like sin, cos, etc.
- load a file with a list of commands

You can add more functions and constants to the tool, if you edit the file **preload.c**.

## Compilation

```
  bash compile.sh
```

## Run

```
  ./CLIMath.out
```