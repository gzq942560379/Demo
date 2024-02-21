#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "gemm_test.h"
#include "slave_kernel.h"

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

void convert_float2half(short* fp16, const float* fp32, uint64_t len){
    convert_float2half_param_t para;
    para.len = len;
    para.fp32 = fp32;
    para.fp16 = fp16;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(convert_float2half)), &para);
    CRTS_athread_join();
}

void convert_half2float(float* fp32, const short* fp16, uint64_t len){
    convert_half2float_param_t para;
    para.len = len;
    para.fp16 = fp16;
    para.fp32 = fp32;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(convert_half2float)), &para);
    CRTS_athread_join();
}

void convert_check(){
    uint64_t len = 64 * 1024;
    float* fp32_in = new float[len];
    short* fp16 = new short[len];
    float* fp32_out = new float[len];

    for(uint64_t i = 0; i < len; ++i){
        fp32_in[i] = i % 32768;
    }

    printf("convert_float2half begin ...\n");
    fflush(stdout);
    
    convert_float2half(fp16, fp32_in, len);
    
    printf("convert_float2half end ...\n");
    fflush(stdout);
    
    printf("convert_half2float begin ...\n");
    fflush(stdout);
    
    convert_half2float(fp32_out, fp16, len);

    printf("convert_half2float end ...\n");
    fflush(stdout);

    for(uint64_t i = 0; i < len; ++i){
        printf("%d : %f, %f\n", i, fp32_in[i], fp32_out[i]);
        fflush(stdout);
    }

    delete[] fp32_in;
    delete[] fp32_out;
    delete[] fp16;
}

void hgemm_check(){

    int m = 1024;
    int n = 1024;
    int k = 1024;

    int M,N,K;
    int lda,ldb,ldc;
    float alpha, beta;
    char transA = 'N';
    char transB = 'N';
    M = m;
    N = n;
    K = k;
    lda = M;
    ldb = K;
    ldc = M;

    Matrix<float16_t> Ah(m, k, lda);
    Matrix<float16_t> Bh(k, n, ldb);
    Matrix<float16_t> Ch(m, n, ldc);

    Matrix<float> Af(m, k, lda);
    Matrix<float> Bf(k, n, ldb);
    Matrix<float> Cf(m, n, ldc);
    Matrix<float> Cf_answer(m, n, ldc);


    Af.fill(1);
    Bf.fill(1);
    Cf.fill(1);
    Cf_answer.fill(1);

    convert_float2half(Ah.data(), Af.data(), m * k);
    convert_float2half(Bh.data(), Bf.data(), k * n);
    convert_float2half(Ch.data(), Cf.data(), m * n);

    alpha=1.;
    beta=1.;

    float16_t a = 1;
    float16_t b = 1;

    gemm(transA, transB, a, Ah, Bh, b, Ch);

    gemm(transA, transB, alpha, Af, Bf, beta, Cf_answer);

    convert_half2float(Cf.data(), Ch.data(), m * n);

    for(int i = 0; i < m * n; ++i){
        printf("%f %f \n", Cf_answer.data()[i], Cf.data()[i]);
    }
}

int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    // printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(T)","FLOPS(T)","peak(%)");
    
    // gemm_performance_test<float16_t>(1024,1024,1024);
    // gemm_performance_test<float16_t>(2048,2048,2048);
    // gemm_performance_test<float16_t>(4096,4096,4096);
    // gemm_performance_test<float16_t>(8192,8192,8192);
    // gemm_performance_test<float16_t>(16384,1600,3200);

    hgemm_check();

    MPI_Finalize();
    return 0;
}
