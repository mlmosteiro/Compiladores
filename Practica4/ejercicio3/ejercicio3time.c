#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define N 10000


struct timeval inicio, segundo, final;
double tiempo;
double res[N];
int main(int argc, char *argv[]) {
	int i, maxIt;
	double x;

	if (argc<2){
		printf("[ERROR] Indicar numero de iteraciones como argumento\n");
		return -1;
	} else{
		maxIt = atoi(argv[1]);
		tiempo=0;
	}


	for(i=0;i<N;i++)
		res[i]=0.0005*i;

	for(int it=0; it<maxIt; it++){
	   	gettimeofday(&inicio,NULL);

		for(i=0;i<N;i++) {
			x=res[i];
			if (x<10.0e6) x=x*x+0.0005;
			else x=x-1000;
			res[i]+=x;
		}

		gettimeofday(&final,NULL);
   		tiempo += final.tv_sec-inicio.tv_sec+(final.tv_usec-inicio.tv_usec)/1.e6;
	}

	double aux = tiempo/(double)maxIt;
	printf("------------------------------------------\n");
	printf("Tamaño de N: %d\n", N);
	printf("Tiempo medio: %lf\n", aux );
	printf("resultado= %e\n", res[N-1]);
	return 0;
}