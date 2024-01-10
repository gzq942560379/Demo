#pragma once
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <complex>
#include <exception>
#include <cstring>
#include <string>
#include <iostream>
#include <cblas.h>
#include <cassert>

#ifdef __cplusplus
extern "C"{
#endif

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
            data_ = (T*)aligned_alloc(64, n_ * ld_ * sizeof(T));
        }else{
            assert(ld_ >= n_);
            data_ = (T*)aligned_alloc(64, m_ * ld_ * sizeof(T));
        }
    }
    ~Matrix(){
        free(data_);
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
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, A.m(), B.n(), A.n(), alpha, A.data(), A.ld(), B.data(), B.ld(), beta, C.data(), C.ld());
}

template<>
inline void gemm<__fp16>(char transA, char transB, __fp16 alpha, const Matrix<__fp16>& A, const Matrix<__fp16>& B, __fp16 beta, Matrix<__fp16>& C){
    assert(transA == 'N');
    assert(transB == 'N');
    assert(A.n() == B.m());
    fjcblas_gemm_r16(CblasColMajor, CblasNoTrans, CblasNoTrans, A.m(), B.n(), A.n(), alpha, A.data(), A.ld(), B.data(), B.ld(), beta, C.data(), C.ld());
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

    A.fill(1.);
    B.fill(1.);
    C.fill(1.);

    alpha=1.;
    beta=0.;

    // warm up
    // gemm( transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc );
    gemm(transA, transB, alpha, A, B, beta, C);

    double timer_start = omp_get_wtime();

    gemm(transA, transB, alpha, A, B, beta, C);
    
    double timer_end = omp_get_wtime();

    double time = timer_end - timer_start;
    double TFLOPs = 2. * M * N * K * 1e-12;
    double TFLOPS = TFLOPs / time;

    double THEORETICAL_PEAK_PROC_TFLOPS = env_get_double("THEORETICAL_PEAK_PROC_TFLOPS");
    double peak = TFLOPS / THEORETICAL_PEAK_PROC_TFLOPS * 100;

    double memory = 1.0 * (M * N + M * K + K * N) * sizeof(T) /1024/1024/1024;
    printf("%8d,%8d,%8d,%12.4e,%12.4e,%12.4e,%12.4e,%12.2f%%\n",M,K,N,memory,time,TFLOPs,TFLOPS, peak);
}
