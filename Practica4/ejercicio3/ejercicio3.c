#include <stdio.h>
#define N 10000
double res[N];
int main() {
	int i;
	double x;
	for(i=0;i<N;i++)
		res[i]=0.0005*i;
	for(i=0;i<N;i++) {
		x=res[i];
		if (x<10.0e6) x=x*x+0.0005;
		else x=x-1000;
		res[i]+=x;
	}
	printf("resultado= %e\n", res[N-1]);
	return 0;
}