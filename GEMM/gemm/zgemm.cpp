#include <cstdio>
#include <cstdlib>
#include <complex>
#include <omp.h>

#ifdef __cplusplus
extern "C"{
#endif

extern void zgemm_(char* transA, char* transB, int* m, int* n, int* k, std::complex<double>* alpha, std::complex<double>* a, int* lda, std::complex<double>* b, int* ldb, std::complex<double>* beta, std::complex<double>* c, int* ldc);

#ifdef __cplusplus
}
#endif

void call_zgemm(char transA, char transB, int M, int N, int K, std::complex<double> alpha, std::complex<double> *A,int lda, std::complex<double>* B, int ldb, std::complex<double> beta, std::complex<double>* C, int ldc){
    zgemm_( &transA, &transB, &M, &N, &K,
            &alpha, A, &lda,
            B, &ldb,
            &beta, C, &ldc );
}

void zgemm_performance_test(int m, int n, int k){
    int M,N,K;
    int lda,ldb,ldc;
    std::complex<double> alpha,beta;
    std::complex<double> *A,*B,*C;
    char transA = 'N';
    char transB = 'N';
    M = m;
    N = n;
    K = k;
    lda = M;
    ldb = K;
    ldc = M;
    A = (std::complex<double>*)malloc(M * K * sizeof(std::complex<double>));
    B = (std::complex<double>*)malloc(K * N * sizeof(std::complex<double>));
    C = (std::complex<double>*)malloc(M * N * sizeof(std::complex<double>));

    for(int c = 0; c < K; c++){
        for(int r = 0;r < M; r++){
            A[c * lda + r] = std::complex<double>(1.0, 0.0);
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < K; r++){
            B[c * lda + r] = std::complex<double>(1.0, 0.0);
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < M; r++){
            C[c * lda + r] = std::complex<double>(1.0, 0.0);
        }
    }
    alpha = std::complex<double>(1.0, 0.0);
    beta = std::complex<double>(1.0, 0.0);

    double timer_start = omp_get_wtime();
    call_zgemm( transA, transB, M, N, K,
                alpha, A, lda,
                B, ldb,
                beta, C, ldc );
    double timer_end = omp_get_wtime();

    double time = timer_end - timer_start;
    double FLOPs = 8.*M*N*K*1e-9;
    double FLOPS = FLOPs/time;
    double memory = 1.0 * (M * N + M * K + K * N) * sizeof(std::complex<double>) /1024/1024/1024;
    printf("%8d,%8d,%8d,%12.4e,%12.4e,%12.4e,%12.4e\n",M,K,N,memory,time,FLOPs,FLOPS);

    free(A);
    free(B);
    free(C);
}

int main(int argc,char* argv[]){


    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(G)","FLOPS(G)");

    zgemm_performance_test(8,8,8);
    zgemm_performance_test(16,16,16);
    zgemm_performance_test(32,32,32);
    zgemm_performance_test(64,64,64);
    zgemm_performance_test(128,128,128);
    zgemm_performance_test(256,256,256);
    zgemm_performance_test(512,512,512);
    zgemm_performance_test(1024,1024,1024);
    zgemm_performance_test(2048,2048,2048);
    // zgemm_performance_test(4096,4096,4096);

    return 0;
}