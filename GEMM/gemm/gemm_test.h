#pragma once
#include <cstdio>
#include <cstdlib>
#include <complex>
#include <exception>
#include <cstring>
#include <string>
#include <iostream>

#ifdef USE_CBLAS
#include <cblas.h>
#endif

#ifdef __sw_64__
#include <swhgemm.h>
typedef double float64_t;
typedef float float32_t;
typedef short float16_t;
#endif

#include <cassert>
#include <mpi.h>

#ifdef __cplusplus
extern "C"{
#endif

extern void sgemm_(char* transA, char* transB, int* m, int* n, int* k, float* alpha, const float* a, int* lda, const float* b, int* ldb, float* beta, float* c, int* ldc);
extern void dgemm_(char* transA, char* transB, int* m, int* n, int* k, double* alpha, const double* a, int* lda, const double* b, int* ldb, double* beta, double* c, int* ldc);
extern void zgemm_(char* transA, char* transB, int* m, int* n, int* k, std::complex<double>* alpha, const std::complex<double>* a, int* lda, const std::complex<double>* b, int* ldb, std::complex<double>* beta, std::complex<double>* c, int* ldc);

#ifdef __cplusplus
}
#endif

static bool env_get_bool(const char* name, bool default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    if(std::strcmp(tmp, "true") == 0){
        return true;
    }else{
        return false;
    }
}

static int env_get_int(const char* name, int default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return std::atoi(tmp);
}

static double env_get_double(const char* name, double default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return std::atof(tmp);
}

static double env_get_double(const char* name){
    char* tmp = getenv(name);
    if(tmp == NULL){
        throw std::invalid_argument("env variable not found !!!");
    }
    return std::atof(tmp);
};

template<typename T>
class Matrix{

public:

    enum Layout {RowMajor, ColMajor};

    int m_;
    int n_;
    int ld_;
    Layout layout_;
    T* data_;

    Matrix(int m, int n, int ld, Matrix::Layout layout = ColMajor):m_(m),n_(n),ld_(ld),layout_(layout){
        if(layout_ == ColMajor){
            assert(ld_ >= m_);
            data_ = (T*)libc_aligned_malloc(n_ * ld_ * sizeof(T));
            // data_ = (T*)malloc(n_ * ld_ * sizeof(T));
            // data_ = new T[n_ * ld_];
        }else{
            assert(ld_ >= n_);
            data_ = (T*)libc_aligned_malloc(m_ * ld_ * sizeof(T));
            // data_ = (T*)malloc(m_ * ld_ * sizeof(T));
            // data_ = new T[m_ * ld_];
        }
    }
    ~Matrix(){
        // free(data_);
        delete[] data_;
    }

    int m() const {return m_;};
    int n() const {return n_;};
    int ld() const {return ld_;};
    Layout layout() const {return layout_;};
    const T* data() const {return data_;};
    T* data() {return data_;};

    void fill(T value){
        if(layout_ == ColMajor){
            for(int c = 0; c < n_; ++c){
                for(int r = 0; r < m_; ++r){
                    data_[r + c * ld_] = value;
                }
            }
        }else{
            for(int r = 0; r < m_; ++r){
                for(int c = 0; c < n_; ++c){
                    data_[c + r * ld_] = value;
                }
            }    
        }
    }


};

template<typename T>
inline void gemm(char transA, char transB, T alpha, const Matrix<T>& A, const Matrix<T>& B, T beta, Matrix<T>& C);

template<>
inline void gemm<double>(char transA, char transB, double alpha, const Matrix<double>& A, const Matrix<double>& B, double beta, Matrix<double>& C){
    assert(transA == 'N');
    assert(transB == 'N');
    assert(A.n() == B.m());
#ifdef USE_CBLAS
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, A.m(), B.n(), A.n(), alpha, A.data(), A.ld(), B.data(), B.ld(), beta, C.data(), C.ld());
#else
    int m = A.m();
    int n = B.n();
    int k = A.n();
    int lda = A.ld();
    int ldb = B.ld();
    int ldc = C.ld();
    dgemm_(&transA, &transB, &m, &n, &k, &alpha, A.data(), &lda, B.data(), &ldb, &beta, C.data(), &ldc);
#endif
}

template<>
inline void gemm<float>(char transA, char transB, float alpha, const Matrix<float>& A, const Matrix<float>& B, float beta, Matrix<float>& C){
    assert(transA == 'N');
    assert(transB == 'N');
    assert(A.n() == B.m());
#ifdef USE_CBLAS
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, A.m(), B.n(), A.n(), alpha, A.data(), A.ld(), B.data(), B.ld(), beta, C.data(), C.ld());
#else
    int m = A.m();
    int n = B.n();
    int k = A.n();
    int lda = A.ld();
    int ldb = B.ld();
    int ldc = C.ld();
    sgemm_(&transA, &transB, &m, &n, &k, &alpha, A.data(), &lda, B.data(), &ldb, &beta, C.data(), &ldc);
#endif
}



template<typename T>
void gemm_performance_test(int m, int n, int k){

    int M,N,K;
    int lda,ldb,ldc;
    T alpha, beta;
    char transA = 'N';
    char transB = 'N';
    M = m;
    N = n;
    K = k;
    lda = M;
    ldb = K;
    ldc = M;

    Matrix<T> A(m, k, lda);
    Matrix<T> B(k, n, ldb);
    Matrix<T> C(m, n, ldc);

    A.fill(0);
    B.fill(0);
    C.fill(0);

    alpha=1.;
    beta=0.;

    // warm up
    // gemm( transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc );
    gemm(transA, transB, alpha, A, B, beta, C);

    double timer_start = MPI_Wtime();

    gemm(transA, transB, alpha, A, B, beta, C);
    
    double timer_end = MPI_Wtime();

    double time = timer_end - timer_start;
    double TFLOPs = 2. * M * N * K * 1e-12;
    double TFLOPS = TFLOPs / time;

    double THEORETICAL_PEAK_PROC_TFLOPS = env_get_double("THEORETICAL_PEAK_PROC_TFLOPS");
    double peak = TFLOPS / THEORETICAL_PEAK_PROC_TFLOPS * 100;

    double memory = 1.0 * (M * N + M * K + K * N) * sizeof(T) /1024/1024/1024;
    printf("%8d,%8d,%8d,%12.4e,%12.4e,%12.4e,%12.4e,%12.2f%%\n",M,K,N,memory,time,TFLOPs,TFLOPS, peak);
}
