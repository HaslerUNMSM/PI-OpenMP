#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define VECES 10
#define N 4

double durSecuencial[N], durParalelo[N], sumPromSecuencial[N], sumPromParalela[N];
int n[N] = {10000, 100000, 1000000, 10000000};

void piSecuencial(){
    int dimension;
    double sumSecuencial,tini,dur;
    int i,j;
    
    for (dimension = 0; dimension < N; dimension++)
    {
        dur = 0.0;
        for (i = 0; i < VECES; i++)
        {
            tini = omp_get_wtime();
            sumSecuencial = 0;
            for (j = 0; j < n[dimension]; j++)
            {
                sumSecuencial += 4 * (pow(-1, j) / (2 * j + 1));
            }
            dur += (omp_get_wtime() - tini);
        }
        sumPromSecuencial[dimension] = sumSecuencial;
        durSecuencial[dimension] = dur / VECES;
    }
}

void piParalelo(int HILOS){
    int dimension;
    double sumaParalela,tini,dur;
    int i,j;
    for (dimension = 0; dimension < N; dimension++)
    {
        dur = 0.0;
        for (i = 0; i < VECES; i++)
        {
            sumaParalela = 0;
            tini = omp_get_wtime();
            #pragma omp parallel for private(j) reduction(+ : sumaParalela)  num_threads(HILOS)
            for (j = 0; j < n[dimension]; j++)
            {
                sumaParalela += 4.0 * pow(-1, j) / (2 * j + 1);
            }
            dur += (omp_get_wtime() - tini);
        }
        sumPromParalela[dimension] = sumaParalela;
        durParalelo[dimension] = dur / VECES;
    }
}

int main(int argc, char **argv)
{
    int dimension,i;
    double durParalelo2[N],durParalelo4[N];
    piSecuencial();
    piParalelo(2);
    for (i= 0; i < N; i++){
        durParalelo2[i]=durParalelo[i];
    }
    for (i= 0; i < N; i++){
        durParalelo[i]=0;
    }
    piParalelo(4);
    for (int i= 0; i < N; i++){
        durParalelo4[i]=durParalelo[i];
    }
    printf("\n%s,%s,%s,%s,%s,%s,%s\n","-","Cantidad","TSecuencial","TParalelo2","TParalelo4","ValorPI","ValorPI");
    for (dimension = 0; dimension < N; dimension++){
        printf("%i,%i,%f,%f,%f,%f,%f\n",dimension,n[dimension],durSecuencial[dimension],durParalelo2[dimension],durParalelo4[dimension],sumPromSecuencial[dimension],sumPromParalela[dimension]);
    }
}
