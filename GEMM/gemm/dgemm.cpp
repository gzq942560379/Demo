#include <cstdio>
#include <cstdlib>
#include <mpi.h>

#ifdef __cplusplus
extern "C"{
#endif

extern void dgemm_(char* transA, char* transB, int* m, int* n, int* k, double* alpha, double* a, int* lda, double* b, int* ldb, double* beta, double* c, int* ldc);

#ifdef __cplusplus
}
#endif

void call_dgemm(char transA, char transB, int M, int N, int K, double alpha, double *A,int lda, double* B, int ldb, double beta, double* C, int ldc){
    dgemm_( &transA, &transB, &M, &N, &K,
            &alpha, A, &lda,
            B, &ldb,
            &beta, C, &ldc);
}

void dgemm_performance_test(int m, int n, int k){
    int M,N,K;
    int lda,ldb,ldc;
    double alpha,beta;
    double *A,*B,*C;
    char transA = 'N';
    char transB = 'N';
    M = m;
    N = n;
    K = k;
    lda = M;
    ldb = K;
    ldc = M;
    A = (double*)malloc(M * K * sizeof(double));
    B = (double*)malloc(K * N * sizeof(double));
    C = (double*)malloc(M * N * sizeof(double));

#ifdef _RANDOM
    srand(1);
    for(int c = 0; c < K; c++){
        for(int r = 0;r < M; r++){
            A[c * lda + r] = rand() % 10;
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < K; r++){
            B[c * ldb + r] = rand() % 10;
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < M; r++){
            C[c * lda + r] = rand() % 10;
        }
    }
    alpha=rand() % 10;
    beta=rand() % 10;
#else
    for(int c = 0; c < K; c++){
        for(int r = 0;r < M; r++){
            A[c * lda + r] = 1.;
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < K; r++){
            B[c * ldb + r] = 1.;
        }
    }
    for(int c = 0; c < N; c++){
        for(int r = 0;r < M; r++){
            C[c * lda + r] = 1.;
        }
    }
    alpha=1.;
    beta=1.;
#endif

    double timer_start = MPI_Wtime();
    call_dgemm( transA, transB, M, N, K,
                alpha, A, lda,
                B, ldb,
                beta, C, ldc );
    double timer_end = MPI_Wtime();

    double time = timer_end - timer_start;
    double FLOPs = 2.*M*N*K*1e-9;
    double FLOPS = FLOPs/time;
    double memory = 1.0 * (M * N + M * K + K * N) * sizeof(double) /1024/1024/1024;
    printf("%8d,%8d,%8d,%12.8lf,%12.6lf,%12.6lf,%12.4lf\n",M,K,N,memory,time,FLOPs,FLOPS);

    free(A);
    free(B);
    free(C);
}

int main(int argc,char* argv[]){

    MPI_Init(&argc, &argv);

    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(G)","FLOPS(G)");
    dgemm_performance_test(8,8,8);
    dgemm_performance_test(16,16,16);
    dgemm_performance_test(32,32,32);
    dgemm_performance_test(64,64,64);
    dgemm_performance_test(128,128,128);
    dgemm_performance_test(256,256,256);
    dgemm_performance_test(512,512,512);
    dgemm_performance_test(1024,1024,1024);
    dgemm_performance_test(2048,2048,2048);
    dgemm_performance_test(4096,4096,4096);
    dgemm_performance_test(8192,8192,8192);
    dgemm_performance_test(16384,16384,16384);

    MPI_Finalize();
    return 0;
}