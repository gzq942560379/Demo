#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "gemm_test.h"


int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","K","N","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");

    // gemm_performance_test<float>(1024,1024,1024);
    // gemm_performance_test<float>(2048,2048,2048);
    // gemm_performance_test<float>(4096,4096,4096);
    // gemm_performance_test<float>(8192,8192,8192);
    // gemm_performance_test<float>(16384,1600,3200);

    gemm_performance_test<float>(3200,32768,1600);
    gemm_performance_test<float>(1600,32768,3200);

    MPI_Finalize();
    return 0;
}
