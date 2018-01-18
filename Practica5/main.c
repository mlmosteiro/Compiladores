#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#define ITER 100000
#define N 10
/*
Formato de los resultados
	- Tamaño de N 
	- Tiempo total de todas las iteraciones sin aplicar la optimizacion
	- Tiempo medio de todas las iteraciones sin aplicar la optimizacion
	- Valor de un elemento de X (Para evitar codigo muerto)
	- Valor de un elemento de Y (Para evitar codigo muerto)
	- Tiempo total optimizado de todas las iteraciones
	- Tiempo medio optimizado de todas las iteraciones
	- Valor de un elemento de X (Para evitar codigo muerto)
	- Valor de un elemento de Y (Para evitar codigo muerto)
*/

struct timeval inicio, segundo, final;
double tiempo;

int main() {
	int i, j, k;
	static float x[N], y[N];
	tiempo=0;
	
	//Calentamiento de cache
	for(i=0; i<N; i++) {
		x[i] = (float)  i;
		y[i] = (float)  i;
	}


	for(k=0; k<ITER; k++){
		gettimeofday(&inicio,NULL);
		for(i=0; i<N; i++){
			x[i] = sqrt((float)i);
		}	
		for(i=0; i<N; i++){
			y[i] = (float)i+2.0;
		}

		gettimeofday(&final,NULL);
		tiempo += final.tv_sec-inicio.tv_sec+(final.tv_usec-inicio.tv_usec)/1.e6;
	}
	i--;
	double aux = tiempo/(double)ITER;
	printf("%d; %lf; %lf;", N, tiempo, aux);
	printf("%f; %f;", x[i], y[i]);
	aux=0;
	tiempo=0;
   	
	for(k=0; k<ITER; k++)
{		gettimeofday(&inicio,NULL);
		for(i=0; i<N; i++) {
			x[i] = sqrt((float)i);
			y[i] = (float)i+2.0;
		}

		gettimeofday(&final,NULL);
		tiempo += final.tv_sec-inicio.tv_sec+(final.tv_usec-inicio.tv_usec)/1.e6;
	}
	i--;
	aux = tiempo/(double)ITER;
	printf("%lf; %lf;",tiempo, aux);
	printf("%f; %f \n", x[i], y[i]);

	return 0;
}