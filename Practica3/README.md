### Consola matemática V1.0


## Practica 3 - Objetivo
Construir un una herramienta que permita compilar y ejecutar ficheros con secuencias de expresiones de cálculo matemático.
Asignatura **Compiladores e Interpretes**, Universidad de Santiago de Compostela.

## Descripción
Se trata de una consola capaz de interpretar y ejecutar comandos matemáticos basicos.

- Operaciones aritméticas +,-,*,/
- Operaciones matemáticas sin,cos,tan,atan, ...
- Declaración y manejo de variables
- Ejecucion de scripts con comandos matemáticos

Además, la lista de funciones disponibles se puede ampliar o modificar editando el archivo Definitions.c

## Instrucciones:
	1. Para compilar, utilizar el script compile.sh:
		
		```
		bash compile.sh
		```
		o
		```
		chmod +x compile.sh
		./compile.sh
		```
		Es necesario tener instaladas las librerias de FLEX y BISON.

	2. Para ejecutar utilizar la siguiente sintaxis
			```
			./ConsolaMatematicaV1.0 
			```
			o
			```
			./ConsolaMatematicaV1.0 <scriptOpcional>
			```
	3. Para eliminar los archivos generados por la compilacion
		```
	    make clean
	    make cleanall
		```
