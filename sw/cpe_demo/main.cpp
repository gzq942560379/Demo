#include <iostream>
#include <mpi.h>
#include <crts.h>
#include <math.h>
#include <cstdint>

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

void fp16_test(){
    float value[64];
    fp16_demo_param_t para;
    para.value = value;
    for(int i = 0; i < 64; ++i){
        value[i] = i;
    }

    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_demo)), &para);
    CRTS_athread_join();

    // printf("master : \n");
    // for(int i = 0; i < 64; ++i){
    //     printf("value[%d] : %f\n", i, value[i]);
    // }
}

// belong 0 or 1
// 0 - 31
void get_simd_vshfh_mask(char belong[32], char id[32]){
    int32_t maskbit[6] = {
        0, 0, 0, 0, 0, 0
    };
    // 32个数
    for(int i = 0; i < 32; ++i){
        int32_t tmp = 0;
        if(belong[i] == 0){
            // do nothing
        }else if(belong[i] == 1){
            tmp |= 1 << 5;
        }else{
            assert(false);
        }
        assert(id[i] < 32);
        tmp |= (id[i] & 0x1F);
        assert(tmp < 64);

        int start_bit = i * 6;
        int end_bit = (i + 1) * 6 - 1;

        int start_char = start_bit / 32;
        int start_pos = start_bit % 32;
        int end_char = end_bit / 32;
        int end_pos = end_bit % 32;


        if(start_char == end_char){
            maskbit[start_char] |= (tmp << start_pos);
        }else{
            maskbit[start_char] |= (tmp << start_pos);
            maskbit[end_char] |= ((tmp >> (6 - (end_pos + 1))) & ((1 << (end_pos + 1)) - 1));
        }
    }
    
    for(int i = 0; i < 6; ++i){
        printf("%d", maskbit[i]);
        if(i + 1 < 6){
            printf(", ");
        }
    }
    printf("\n");


}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    CRTS_init();

    // ssync_demo_test();

    ldm_info_demo_test();

    // axpy_test();

    // rma_demo_test();


    // char belong[32] = {
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    // };
    // char id[32] = {
    //     0, 4, 8, 12, 16, 20, 24, 28,
    //     0, 4, 8, 12, 16, 20, 24, 28,
    //     0, 4, 8, 12, 16, 20, 24, 28,
    //     0, 4, 8, 12, 16, 20, 24, 28,
    // };
    // get_simd_vshfh_mask(belong, id);

    // char belong[32] = {
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    // };
    // char id[32] = {
    //     0, 1, 2, 3, 4, 5, 6, 7,
    //     8, 9, 10, 11, 12, 13, 14, 15,
    //     0, 1, 2, 3, 4, 5, 6, 7,
    //     8, 9, 10, 11, 12, 13, 14, 15,
    // };
    // get_simd_vshfh_mask(belong, id);

    // char belong[32] = {
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     0, 0, 0, 0, 0, 0, 0, 0, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    //     1, 1, 1, 1, 1, 1, 1, 1, 
    // };
    // char id[32] = {
    //     0, 8,  16, 24,
    //     1, 9,  17, 25,
    //     2, 10, 18, 26,
    //     3, 11, 19, 27,
    //     4, 12, 20, 28,
    //     5, 13, 21, 29,
    //     6, 14, 22, 30,
    //     7, 15, 23, 31,
    // };
    // get_simd_vshfh_mask(belong, id);


    // fp16_test();

    double a = std::nan("1");
    printf("%lf %d \n", a, std::isnan(a));

    MPI_Finalize();
    return 0;
}