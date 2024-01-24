#include <cstdio>
#include <cstdlib>
#include "gemm_test.h"

#ifdef __sw_64__
#include <crts.h>
#endif


int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");

    gemm_performance_test<double>(1024,1024,1024);
    gemm_performance_test<double>(2048,2048,2048);
    gemm_performance_test<double>(4096,4096,4096);
    gemm_performance_test<double>(8192,8192,8192);
    gemm_performance_test<double>(16384,1600,3200);

    MPI_Finalize();
    return 0;
}
