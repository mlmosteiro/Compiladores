/*Generación y optimizacion de código*/
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

struct timeval inicio, final;
double tiempoAritmetico;
double tiempoEntradaSalida;
double operacionesAritmeticas();
void operacionesEntradaSalida();

/*Ejercicio 1:
* Mide el tiempo de ejecución con dos ejemplos simples: uno con muchas
* operaciones aritméticas, y otro con muchas operaciones de entrada/salida. Haz
* varias medidas.
*/

#define NUMERO_OPERACIONES 1000
int main(){
	double aux;
	gettimeofday(&inicio,NULL);
	for (int i = 0; i<NUMERO_OPERACIONES; i++){
		aux = operacionesAritmeticas();
	}
	gettimeofday(&final,NULL);
	tiempoAritmetico = (final.tv_sec-inicio.tv_sec+(final.tv_usec-inicio.tv_usec)/1.e6);


	gettimeofday(&inicio,NULL);
	for (int i = 0; i<NUMERO_OPERACIONES; i++){
		operacionesEntradaSalida();
	}
	gettimeofday(&final,NULL);
	tiempoEntradaSalida = (final.tv_sec-inicio.tv_sec+(final.tv_usec-inicio.tv_usec)/1.e6);
	
	printf("============================================================\n");
	printf("tiempoAritmetico 1000*1000  : %f \n", tiempoAritmetico);
	printf("tiempoEntradaSalida 1000*1000  : %f \n", tiempoEntradaSalida);

}



double operacionesAritmeticas(){
	double aux = 0;
	for( int i =0; i<NUMERO_OPERACIONES; i++){
		aux = pow(cos(aux),NUMERO_OPERACIONES);
	}
	return aux;
}

void operacionesEntradaSalida(){
	for( int i =0; i<NUMERO_OPERACIONES; i++){
		printf("Prueba\n");
	}
}