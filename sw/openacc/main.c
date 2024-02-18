#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// #define len 1024*1024*128

// double a[len];
// double b[len];
// double c[len];

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int len = 1024*1024*128;
    
    double* a = (double*)malloc(len * sizeof(double));
    double* b = (double*)malloc(len * sizeof(double));
    double* c = (double*)malloc(len * sizeof(double));

    for(int i = 0; i < len; ++i){
        a[i] = 1;
        b[i] = 2;
        c[i] = 3;
    }

    double start = MPI_Wtime();

    #pragma acc parallel loop tile(1024) copyin(len ,a, b,c) annotate(readonly=(len, b,c)) copyout(a) annotate(dimension(a(len),b(len),c(len)))
    for(int i = 0; i < len; ++i){
        a[i] += b[i] * c[i];
    }

    double end = MPI_Wtime();

    double time = end - start;

    printf("time : %lf\n", time);


    for(int i = 0; i < 10; ++i){
        printf("a[%d] : %lf\n", i, a[i]);
    }
    
    MPI_Finalize();
    return 0;
}

