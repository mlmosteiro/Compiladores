#!/bin/bash

echo "Preprocesado -E">> res.txt
gcc -E ejercicio2.c -o preprocesado.out
tamano=`stat -c %s preprocesado.out`
echo "Tamaño del archivo $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Ensamblador -S">> res.txt
gcc -S ejercicio2.c -o ensamblador.out
tamano=`stat -c %s ensamblador.out`
echo "Tamaño del archivo $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt 

echo "Objeto -c">> res.txt
gcc -c ejercicio2.c -o objeto.out
tamano=`stat -c %s objeto.out`
echo "Tamaño del archivo $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Enlazado Estático -static">> res.txt
gcc -static ejercicio2.c -o estatico.out
tamano=`stat -c %s estatico.out`
echo "Tamaño del archivo $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Enlazado Dinámico -static">> res.txt
gcc ejercicio2.c -o dinamico.out
tamano=`stat -c %s dinamico.out`
echo "Tamaño del archivo $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

NUMERO_ITERACIONES=100

touch res.txt
echo "Optimizacion -O0">> res.txt
gcc -O0 -c ejercicio2.c -o ejercicio2_O0.o
gcc -S -O0 ejercicio2.c -o ejercicio2_O0_ensamblador.out
gcc -O0 ejercicio2time.c -o ejecutable
./ejecutable $NUMERO_ITERACIONES>> res.txt
tamano=`stat -c %s ejercicio2_O0.o`
echo "Tamaño del objeto $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Optimizacion -O1">> res.txt
gcc -O1 -c ejercicio2.c -o ejercicio2_O1.o
gcc -S -O1 ejercicio2.c -o ejercicio2_O1_ensamblador.out
gcc -O1 ejercicio2time.c -o ejecutable
./ejecutable $NUMERO_ITERACIONES>> res.txt
tamano=`stat -c %s ejercicio2_O1.o`
echo "Tamaño del objeto $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Optimizacion -O2">> res.txt
gcc -O2 -c ejercicio2.c -o ejercicio2_O2.o
gcc -S -O2 ejercicio2.c -o ejercicio2_O2_ensamblador.out
gcc -O2 ejercicio2time.c -o ejecutable
./ejecutable $NUMERO_ITERACIONES>> res.txt
tamano=`stat -c %s ejercicio2_O2.o`
echo "Tamaño del objeto $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Optimizacion -O3">> res.txt
gcc -O3 -c ejercicio2.c -o ejercicio2_O3.o
gcc -S -O3 ejercicio2.c -o ejercicio2_O3_ensamblador.out
gcc -O3 ejercicio2time.c -o ejecutable
./ejecutable $NUMERO_ITERACIONES>> res.txt
tamano=`stat -c %s ejercicio2_O3.o`
echo "Tamaño del objeto $tamano bytes">> res.txt

echo "------------------------------------------">> res.txt

echo "Optimizacion -Os">> res.txt
gcc -Os -c ejercicio2.c -o ejercicio2_Os.o
gcc -S -Os ejercicio2.c -o ejercicio2_Os_ensamblador.out
gcc -Os ejercicio2time.c -o ejecutable
./ejecutable $NUMERO_ITERACIONES>> res.txt
tamano=`stat -c %s ejercicio2_Os.o`
echo "Tamaño del objeto $tamano bytes">> res.txt
