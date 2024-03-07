#include <iostream>
#include <mpi.h>
#include <crts.h>
#include <math.h>

#include "slave_kernel.h"

void fill_random(double* input, int64_t len, double start, double end){
    double range_len = end - start;
    for(int64_t i = 0; i < len; ++i){
        input[i] = (rand() * range_len / RAND_MAX) + start;
    }
}

void fill_n(double* input, int64_t len, double value){
    for(int64_t i = 0; i < len; ++i){
        input[i] = value;
    }
}

void array_copy(double* dst, const double* src, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void check_error(double* answer, const double* check, int64_t len){
    double a = 0.;
    double b = 0.;
    double max_relative_error = 0.;
    double max_absulte_error = 0.;
    for(int64_t i = 0; i < len; ++i){
        double absulte_error = fabs(answer[i] - check[i]);
        double relative_error = absulte_error / answer[i];
        if(relative_error > max_relative_error){
            max_relative_error = relative_error;
            max_absulte_error = absulte_error;
            a = answer[i];
            b = check[i];
        }
    }
    printf("-------------------------------------------------\n");
    printf("a : %lf\n", a);
    printf("b : %lf\n", b);
    printf("max_absulte_error : %e\n", max_absulte_error);
    printf("max_relative_error : %e\n", max_relative_error);
    printf("-------------------------------------------------\n");
}

void axpy(double* y, double alpha, const double* x, double beta, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        y[i] += alpha * x[i];
    }
}

inline void sw_axpy(double* y, double alpha, const double* x, double beta, int64_t len){

#ifdef __sw_64__

    axpy_param_t para;
    para.y = y;
    para.x = x;
    para.alpha = alpha;
    para.beta = beta;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(axpy_naive)), &para);
    CRTS_athread_join();

#else

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int64_t i = 0; i < len; ++i){
        y[i] = alpah * x[i] + beta * y[i];
    }

#endif

}

void axpy_test(){
    int64_t random_len = 2048;
    double* random_list = (double*)libc_aligned_malloc(random_len * sizeof(double));
    fill_random(random_list, random_len, -5, 4);
    
    int64_t len = 128 * 1024 * 1024; // 1 GB
    double* x  = (double*)libc_aligned_malloc(len * sizeof(double));
    double* y = (double*)libc_aligned_malloc(len * sizeof(double));
    double* y_answer = (double*)libc_aligned_malloc(len * sizeof(double));

    double alpha = 2.;
    double beta = 1;

    for(int64_t i = 0; i < len; i += random_len){
        array_copy(x + i, random_list, random_len);
        array_copy(y + i, random_list, random_len);
        array_copy(y_answer + i, random_list, random_len);
    }

    axpy(y_answer, alpha, x, beta, len);

    sw_axpy(y, alpha, x, beta, len);

    check_error(y_answer, y, len);

    libc_aligned_free(random_list);
    libc_aligned_free(x);
    libc_aligned_free(y);
    libc_aligned_free(y_answer);
}

void ldm_info_demo_test(){
    double value[64];
    ldm_info_demo_param_t para;
    para.value = value;
    for(int i = 0; i < 64; ++i){
        value[i] = 1.0 * i;
    }
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(ldm_info_demo)), &para);
    CRTS_athread_join();
}

void ssync_demo_test(){
    double value[64];
    ssync_demo_param_t para;
    para.value = value;
    for(int i = 0; i < 64; ++i){
        value[i] = 1.0 * i;
    }
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(ssync_demo)), &para);
    CRTS_athread_join();
}

void rma_demo_test(){
    double value[64];
    double to_bcast = 1.; 
    rma_demo_param_t para;
    para.value = value;
    para.to_bcast = to_bcast;
    for(int i = 0; i < 64; ++i){
        value[i] = 0.;
    }

    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(rma_demo)), &para);
    CRTS_athread_join();

    for(int i = 0; i < 64; ++i){
        printf("value[%d] : %lf\n", i, value[i]);
    }
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    CRTS_init();

    // ssync_demo_test();

    // ldm_info_demo_test();

    // axpy_test();

    rma_demo_test();

    MPI_Finalize();
    return 0;
}