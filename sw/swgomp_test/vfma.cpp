#include<iostream>
#include<mpi.h>

void vfma(double *a, double *b, double *c, int n){
    #pragma omp target parallel for
    for (int i = 0; i < n; i++) {
        double prod = b[i]*c[i]; //这 样prod会 开 在 线 程 的 栈 上
        a[i] += prod;
    }
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int len = 1024 * 1024 * 64 * 8;

    double* A = new double[len];
    double* B = new double[len];
    double* C = new double[len];

    for(int i = 0; i < len; ++i){
        A[i] = 1;
        B[i] = 2;
        C[i] = 3;
    }
    double timer_start = MPI_Wtime();

    vfma(A, B, C, len);

    double timer_end = MPI_Wtime();

    double time = timer_end - timer_start;

    std::cout << "A[0] : " << A[0] << std::endl;
    std::cout << "A[1] : " << A[1] << std::endl;
    std::cout << std::endl;
    std::cout << "time : " << time << std::endl;

    MPI_Finalize();
}