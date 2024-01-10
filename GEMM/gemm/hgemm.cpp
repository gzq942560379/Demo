#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "gemm_test.h"
#include "fj_tool/fapp.h"


int main(int argc,char* argv[]){

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");
    
    gemm_performance_test<__fp16>(8192,8192,8192);
    gemm_performance_test<__fp16>(16384,1600,3200);

    return 0;
}
