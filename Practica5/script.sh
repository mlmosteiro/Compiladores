#!/bin/bash

# Formato de los resultados
#	- Tamaño de N 
#	- Tiempo total de todas las iteraciones sin aplicar la optimizacion
#	- Tiempo medio de todas las iteraciones sin aplicar la optimizacion
#	- Valor de un elemento de X (Para evitar codigo muerto)
#	- Valor de un elemento de Y (Para evitar codigo muerto)
#	- Tiempo total optimizado de todas las iteraciones
#	- Tiempo medio optimizado de todas las iteraciones
#	- Valor de un elemento de X (Para evitar codigo muerto)
#	- Valor de un elemento de Y (Para evitar codigo muerto)
# El archivo obtenido del script es un csv


arrayN=(10 100 1000 10000 100000)
touch res.csv

echo "N; T. total; T. medio; X; Y; T. total optimizado; T. medio optimizado; X; Y" >> res.csv;

# Optimizacion -O0

for i in "${arrayN[@]}" ; do	
	cat main.c | sed -r "s/define N 10/define N $i/g" >> auxiliar.c; 
	gcc -O0 auxiliar.c -o ejecutable -lm
	./ejecutable >> res.csv
	rm auxiliar.c
done;


# Optimizacion -O1

for i in "${arrayN[@]}" ; do	
	cat main.c | sed -r "s/define N 10/define N $i/g" >> auxiliar.c; 
	gcc -O1 auxiliar.c -o ejecutable -lm
	./ejecutable >> res.csv
	rm auxiliar.c
done;

# Optimizacion -O2

for i in "${arrayN[@]}" ; do	
	cat main.c | sed -r "s/define N 10/define N $i/g" >> auxiliar.c; 
	gcc -O2 auxiliar.c -o ejecutable -lm
	./ejecutable >> res.csv
	rm auxiliar.c
done;

# Optimizacion -O3

for i in "${arrayN[@]}" ; do	
	cat main.c | sed -r "s/define N 10/define N $i/g" >> auxiliar.c; 
	gcc -O3 auxiliar.c -o ejecutable -lm
	./ejecutable >> res.csv
	rm auxiliar.c
done;