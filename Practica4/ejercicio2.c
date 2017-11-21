#include <stdio.h>
#define Nmax 600
void producto(float x, float y, float *z) {
*z=x*y; }
main() {
float A[Nmax][Nmax], B[Nmax][Nmax], C[Nmax][Nmax], t, r;
int i,j,k;
for(i=0;i<Nmax;i++) /* Valores de las matrices */
for(j=0;j<Nmax;j++) {
A[i][j]=(i+j)/(j+1.1);
B[i][j]=(i-j)/(j+2.1); }
for(i=0;i<Nmax;i++) /* Producto matricial */
for(j=0;j<Nmax;j++) {
t=0;
for (k=0;k<Nmax;k++) {
producto(A[i][k],B[k][j],&r);
t+=r; }
C[i][j]=t; }
}