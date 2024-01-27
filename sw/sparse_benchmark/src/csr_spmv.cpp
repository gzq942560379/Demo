#include "csr.hpp"

#define REPEAT_COUNT 1

template <typename IndexType, typename ValueType>
void CSR<IndexType, ValueType>::spmv_benchmark(){
    const int repeat_count = env_get_int("REPEAT_COUNT", 10);
    // init x y
    vector<ValueType> x(row_, 1.);
    vector<ValueType> y(row_, 0.);
    omp_timer timer;
    // warm up
    SpMV(x, y);

    double time_no_warm_up = timer.timeIncrement();

    for(int repeat = 0; repeat < repeat_count; ++repeat){
        SpMV(x, y);
    }

    double time_warm_up = timer.timeIncrement();

    double FLOPs = 2. * nnz_;
    double TFLOPS = FLOPs / time_no_warm_up * 1e-12;

    double FLOPs_repeat = FLOPs * repeat_count;
    double TFLOPS_repeat = FLOPs_repeat / time_warm_up * 1e-12;

    cout << "spmv benchmark -------------" << endl;
#ifdef _OPENMP
    cout << "thread count : " << omp_get_max_threads() << endl;
#endif
    cout << "with out warm up : " << endl;
    cout << "time : " << time_no_warm_up << endl;
    cout << "FLOPs : " << FLOPs << endl;
    cout << "TFLOPS : " << TFLOPS << endl;
    cout << "with warm up and repeat : " << endl;
    cout << "repeat_count : " << repeat_count << endl;
    cout << "time : " << time_warm_up << endl;
    cout << "FLOPs : " << FLOPs_repeat << endl;
    cout << "TFLOPS : " << TFLOPS_repeat << endl;
    cout << "----------------------------" << endl;
}


template <typename IndexType, typename ValueType>
void CSR<IndexType, ValueType>::SpMV(vector<ValueType>& y, const vector<ValueType>& x){
#ifdef _OPENMP
    #pragma omp parallel for 
#endif
    for(IndexType r = 0; r < row_; ++r){
        for(IndexType idx = rowptr_[r]; idx < rowptr_[r+1]; ++idx){
            y[r] += value_[idx] * x[colidx_[idx]];
        }
    }
}

template class CSR<int64_t, double>;
