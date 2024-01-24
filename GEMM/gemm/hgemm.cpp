#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "gemm_test.h"

template<>
inline void gemm<float16_t>(char transA, char transB, float16_t alpha, const Matrix<float16_t>& A, const Matrix<float16_t>& B, float16_t beta, Matrix<float16_t>& C){

#ifdef __sw_64__
    float alpha_tmp = 1.;
    float beta_tmp = 1.;
    int m = A.m();
    int n = B.n();
    int k = A.n();
    int lda = A.ld();
    int ldb = B.ld();
    int ldc = C.ld();
    hgemm_(&transA, &transB, &m, &n, &k, &alpha_tmp, (const void *)A.data(), &lda, (const void *)B.data(), &ldb, &beta_tmp, (short *)C.data(), &ldc);
#else
    assert(transA == 'N');
    assert(transB == 'N');
    assert(A.n() == B.m());
    fjcblas_gemm_r16(CblasColMajor, CblasNoTrans, CblasNoTrans, A.m(), B.n(), A.n(), alpha, A.data(), A.ld(), B.data(), B.ld(), beta, C.data(), C.ld());
#endif
}


int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");
    
    gemm_performance_test<float16_t>(1024,1024,1024);
    gemm_performance_test<float16_t>(2048,2048,2048);
    gemm_performance_test<float16_t>(4096,4096,4096);
    gemm_performance_test<float16_t>(8192,8192,8192);
    gemm_performance_test<float16_t>(16384,1600,3200);

    MPI_Finalize();
    return 0;
}
