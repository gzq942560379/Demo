#include <iostream>
#include <sstream>

#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cstdlib>
#include <cstdio>

#include <omp.h>
#include <cublas.h>
#include <cublas_v2.h>

#ifndef DEVICE_RESET
#define DEVICE_RESET cudaDeviceReset();
#endif
#define checkCudaError(val) _checkCudaError((val), #val, __FILE__, __LINE__)

#define checkCublasStatus(val) _checkCublasStatus((val), #val, __FILE__, __LINE__)

static const char *_cudaGetErrorEnum(cudaError_t error) {
  return cudaGetErrorName(error);
}

template <typename T>
void _checkCudaError(T result, char const *const func, const char *const file,
           int const line) {
  if (result) {
    fprintf(stderr, "CUDA error at %s:%d code=%d(%s) \"%s\" \n", file, line,
            static_cast<unsigned int>(result), _cudaGetErrorEnum(result), func);
    DEVICE_RESET
    // Make sure we call CUDA Device Reset before exiting
    exit(EXIT_FAILURE);
  }
}

template <typename T>
void _checkCublasStatus(T result, char const *const func, const char *const file,
           int const line) {
  if (result) {
    fprintf(stderr, "cublas status at %s:%d code=%d \"%s\" \n", file, line,
            static_cast<unsigned int>(result),func);
    DEVICE_RESET
    // Make sure we call CUDA Device Reset before exiting
    exit(EXIT_FAILURE);
  }
}

using namespace std;

void initialize_a_b_c(double* ha,
					int size_a,
					double* hb,
					int size_b,
					double* hc,
					int size_c)
{
	for (int i = 0; i < size_a; ++i)
	{
		ha[i] = 1.;
	}
	for (int i = 0; i < size_b; ++i)
	{
		hb[i] = 1.0;
	}
	for (int i = 0; i < size_c; ++i)
	{
		hc[i] = 1.0;
	}
}

double call_cu_dgemm(char transA, char transB, int m, int n, int k, double alpha, double *A,int lda, double* B, int ldb, double beta, double* C, int ldc){
	double *da, *db, *dc;
	checkCudaError(cudaMalloc(&da, m * k * sizeof(double)));
	checkCudaError(cudaMalloc(&db, k * n * sizeof(double)));
	checkCudaError(cudaMalloc(&dc, m * n * sizeof(double)));
	checkCudaError(cudaMemcpy(da, A, sizeof(double) * m * k, cudaMemcpyHostToDevice));
	checkCudaError(cudaMemcpy(db, B, sizeof(double) * k * n, cudaMemcpyHostToDevice));
	checkCudaError(cudaMemcpy(dc, C, sizeof(double) * m * n, cudaMemcpyHostToDevice));
	
	float cuda_elapsed;
	cudaEvent_t start_gemm, end_gemm;
	checkCudaError(cudaEventCreate(&start_gemm));
	checkCudaError(cudaEventCreate(&end_gemm));
	
	cublasHandle_t handle;
	checkCublasStatus(cublasCreate(&handle));

	checkCudaError(cudaEventRecord(start_gemm, NULL));

	checkCublasStatus(cublasDgemm(handle, cublasOperation_t::CUBLAS_OP_N, cublasOperation_t::CUBLAS_OP_N, m, n, k, &alpha, da, lda, db, ldb, &beta, dc, ldc));

	checkCudaError(cudaEventRecord(end_gemm, NULL));

	checkCudaError(cudaEventSynchronize(end_gemm));
	
  checkCudaError(cudaEventElapsedTime(&cuda_elapsed, start_gemm, end_gemm));

	checkCudaError(cudaMemcpy(C, dc, sizeof(double) * m * n, cudaMemcpyDeviceToHost));
	
  checkCudaError(cudaEventDestroy(start_gemm));
  checkCudaError(cudaEventDestroy(end_gemm));
	checkCudaError(cudaFree(da));
	checkCudaError(cudaFree(db));
	checkCudaError(cudaFree(dc));

	return cuda_elapsed / 1000.;
}

void cuda_dgemm_performance_test(int m,int n, int k){
	int M,N,K;
  int lda,ldb,ldc;
  double alpha,beta;
  alpha = 1.;
  beta = 1.;
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

	int size_a = m * k;
	int size_b = k * n;
	int size_c = m * n;
	initialize_a_b_c(A, size_a, B, size_b, C, size_c);
	
  double timer_start = omp_get_wtime();
  double time_without_memcpy = call_cu_dgemm( transA, transB, M, N, K,
              alpha, A, lda,
              B, ldb,
              beta, C, ldc );
  double timer_end = omp_get_wtime();

  double FLOPs = 2.*M*N*K*1e-9;
  double memory = 1.0 * (M * N + M * K + K * N) * sizeof(double) /1024/1024/1024;

  double time_with_memcpy = timer_end - timer_start;
  double FLOPS_with_memcpy = FLOPs/time_with_memcpy;

  double FLOPS_without_memcpy = FLOPs/time_without_memcpy;

  printf("%8d,%8d,%8d,%12.4e,%12.4e,%12.4e,%12.4e,%12.4e,%12.4e\n",M,K,N, memory, FLOPs, time_with_memcpy,FLOPS_with_memcpy, time_without_memcpy, FLOPS_without_memcpy);
}

int main(int argc, char *argv[])
{
    printf("%8s,%8s,%8s,%12s,%12s,%12s,%12s,%12s,%12s\n","M","N","K","mem(GB)","time","FLOPs(G) With Memcpy","FLOPS(G) With Memcpy","FLOPs(G) Without Memcpy","FLOPS(G) Without Memcpy");

    cuda_dgemm_performance_test(8,8,8);
    cuda_dgemm_performance_test(16,16,16);
    cuda_dgemm_performance_test(32,32,32);
    cuda_dgemm_performance_test(64,64,64);
    cuda_dgemm_performance_test(128,128,128);
    cuda_dgemm_performance_test(256,256,256);
    cuda_dgemm_performance_test(512,512,512);
    cuda_dgemm_performance_test(1024,1024,1024);
    cuda_dgemm_performance_test(2048,2048,2048);
    cuda_dgemm_performance_test(4096,4096,4096);

	return EXIT_SUCCESS;
}