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

const int MM = 1024;
const int NN = 1024;
const int KK = 1024;
const double ALPHA = 1.0;
const double BETA = 1.0;

void initialize_a_b_c(vector<double> &ha,
					   int size_a,
					   vector<double> &hb,
					   int size_b,
					   vector<double> &hc,
					   int size_c)
{
	srand(size_a);
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

int main(int argc, char *argv[])
{
	int m = MM;
	int n = NN;
	int k = KK;
	double alpha = ALPHA;
	double beta = BETA;

	int lda = m;
	int ldb = k;
	int ldc = m;

	int size_a = m * k;
	int size_b = k * n;
	int size_c = m * n;

	vector<double> ha(size_a);
	vector<double> hb(size_b);
	vector<double> hc(size_c);

	initialize_a_b_c(ha, size_a, hb, size_b, hc, size_c);

	double *da, *db, *dc;

	cout << "malloc start ..." << endl;

    double malloc_start, malloc_end, malloc_time;
    malloc_start = omp_get_wtime();
	checkCudaError(cudaMalloc(&da, size_a * sizeof(double)));
    malloc_end = omp_get_wtime();
    malloc_time = malloc_end - malloc_start;
	cout << "malloc a time : " << malloc_time << "s" << endl;

    malloc_start = omp_get_wtime();
	checkCudaError(cudaMalloc(&db, size_b * sizeof(double)));
    malloc_end = omp_get_wtime();
	cout << "malloc b time : " << malloc_time << "s" << endl;

    malloc_start = omp_get_wtime();
	checkCudaError(cudaMalloc(&dc, size_c * sizeof(double)));
    malloc_end = omp_get_wtime();
	cout << "malloc c time : " << malloc_time << "s" << endl;
	checkCudaError(cudaMemcpy(da, ha.data(), sizeof(double) * size_a, cudaMemcpyHostToDevice));
	checkCudaError(cudaMemcpy(db, hb.data(), sizeof(double) * size_b, cudaMemcpyHostToDevice));
	checkCudaError(cudaMemcpy(dc, hc.data(), sizeof(double) * size_c, cudaMemcpyHostToDevice));

	// timing
	float cuda_elapsed;
	cudaEvent_t start, end;
	checkCudaError(cudaEventCreate(&start));
	checkCudaError(cudaEventCreate(&end));
	cublasHandle_t handle;
	checkCublasStatus(cublasCreate(&handle));

	checkCudaError(cudaEventRecord(start, NULL));

	checkCublasStatus(cublasDgemm(handle, cublasOperation_t::CUBLAS_OP_N, cublasOperation_t::CUBLAS_OP_N, m, n, k, &alpha, da, lda, db, ldb, &beta, dc, ldc));

	checkCudaError(cudaEventRecord(end, NULL));

	checkCudaError(cudaEventSynchronize(end));
	
    checkCudaError(cudaEventElapsedTime(&cuda_elapsed, start, end));

	checkCudaError(cudaMemcpy(hc.data(), dc, sizeof(double) * size_c, cudaMemcpyDeviceToHost));

#ifdef DEBUG
	printMatrix("ha", ha.data(), m, k, lda);
	printMatrix("hb", hb.data(), k, n, ldb);
	printMatrix("hc", hc.data(), m, n, ldc);
#endif

	cout << "cuda time : " << cuda_elapsed << "ms" << endl;

	checkCudaError(cudaFree(da));
	checkCudaError(cudaFree(db));
	checkCudaError(cudaFree(dc));

	return EXIT_SUCCESS;
}