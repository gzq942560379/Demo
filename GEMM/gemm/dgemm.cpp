#include <cstdio>
#include <cstdlib>
#include "gemm_test.h"

#ifdef __sw_64__
#include <crts.h>
#endif


int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","K","N","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");

    // gemm_performance_test<double>(1024,1024,1024);
    // gemm_performance_test<double>(2048,2048,2048);
    // gemm_performance_test<double>(4096,4096,4096);
    // gemm_performance_test<double>(8192,8192,8192);
    // gemm_performance_test<double>(16384,1600,3200);


    // gemm_performance_test<double>(32768,1600,20);
    // gemm_performance_test<double>(32768,3200,1600);
    // gemm_performance_test<double>(32768,1600,3200);
    // gemm_performance_test<double>(32768,800,1600);
    // gemm_performance_test<double>(32768,400,800);
    // gemm_performance_test<double>(32768,18,400);

    gemm_performance_test<double>(2048,32768,20);
    gemm_performance_test<double>(4096,32768,2048);
    gemm_performance_test<double>(2048,32768,4096);
    gemm_performance_test<double>(1024,32768,2048);
    gemm_performance_test<double>(512,32768,1024);
    gemm_performance_test<double>(20,32768,512);

    gemm_performance_test<double>(1024,32768,3);
    gemm_performance_test<double>(512,32768,1024);
    gemm_performance_test<double>(256,32768,512);
    gemm_performance_test<double>(4,32768,256);

    gemm_performance_test<double>(128,32768,256);
    gemm_performance_test<double>(4,32768,128);

    
    gemm_performance_test<double>(1024,8192,3);
    gemm_performance_test<double>(512,8192,1024);
    gemm_performance_test<double>(256,8192,512);
    gemm_performance_test<double>(4,8192,256);

    gemm_performance_test<double>(128,8192,256);
    gemm_performance_test<double>(4,8192,128);

    MPI_Finalize();
    return 0;
}
