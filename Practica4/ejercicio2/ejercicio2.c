 #include <stdio.h>
#include <stdlib.h>
#define Nmax 600

void producto(float x, float y, float *z) {
	*z=x*y; 
}

int main(int argc, char *argv[]){
	float A[Nmax][Nmax], B[Nmax][Nmax], C[Nmax][Nmax], t, r;
	int i,j,k, maxIt;
	
	if (argc<2){
		printf("[ERROR] Indicar numero de iteraciones como argumento\n");
		return -1;
	} else{
		maxIt = atoi(argv[1]);

	}

	for(i=0;i<Nmax;i++){ /* Valores de las matrices */
		for(j=0;j<Nmax;j++) {
			A[i][j]=(i+j)/(j+1.1);
			B[i][j]=(i-j)/(j+2.1); 
		}
	}

	for(int it=0; it<maxIt; it++){
		for(i=0;i<Nmax;i++){ /* Producto matricial */
			for(j=0;j<Nmax;j++) {
				t=0;
				for (k=0;k<Nmax;k++) {
					producto(A[i][k],B[k][j],&r);
					t+=r; 
				}
			C[i][j]=t;
			}
		}
	}

	printf("Res: %f\n", C[i][j] );
	printf("Número de iteraciones: %d\n", maxIt);
	return 1;
}
