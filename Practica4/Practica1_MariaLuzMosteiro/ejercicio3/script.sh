#!/bin/bash

NUMERO_ITERACIONES=50
arrayN=(10 100 1000 10000 100000 1000000 10000000 100000000)
touch res.txt

echo "Optimizacion -O1">> res.txt
gcc -S -O1 ejercicio3.c -o ejercicio3_O1_ensamblador.out

for i in "${arrayN[@]}" ; do	
	cat ejercicio3time.c | sed -r "s/define N 10000/define N $i/g" >> auxiliar.c; 
	gcc -O1 auxiliar.c -o ejecutable
	./ejecutable $NUMERO_ITERACIONES>> res.txt
	rm auxiliar.c
done;


echo "------------------------------------------">> res.txt

echo "Optimizacion -O1 -funroll-loops">> res.txt
gcc -S -O1 -funroll-loops ejercicio3.c -o ejercicio3_O1_unroll_ensamblador.out

for i in "${arrayN[@]}" ; do	
	cat ejercicio3time.c | sed -r "s/define N 10000/define N $i/g" >> auxiliar.c; 
	gcc -O1 -funroll-loops auxiliar.c -o ejecutable
	./ejecutable $NUMERO_ITERACIONES>> res.txt
	rm auxiliar.c
done;
