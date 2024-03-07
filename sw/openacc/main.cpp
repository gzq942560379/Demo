#include <cstdint>
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    uint64_t len = 1024*1024*128;
    
    double* a = new double[len];
    double* b = new double[len];
    double* c = new double[len];

    for(uint64_t i = 0; i < len; ++i){
        a[i] = 1;
        b[i] = 2;
        c[i] = 3;
    }

    double start = MPI_Wtime();

    #pragma acc parallel loop tile(1024) copyin(len ,a, b,c) annotate(readonly=(len, b,c)) copyout(a) annotate(dimension(a(len),b(len),c(len)))
    for(uint64_t i = 0; i < len; ++i){
        a[i] += b[i] + c[i];
    }

    double end = MPI_Wtime();

    double time = end - start;

    std::cout << "time : " << time << std::endl;

    
    MPI_Finalize();
    return 0;
}

