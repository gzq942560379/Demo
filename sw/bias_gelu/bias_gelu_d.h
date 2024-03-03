#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <crts.h>
#include <mpi.h>
#include "slave_kernel.h"

#define SLAVE_FUN(x) slave_##x 

void fill_random_d(double* input, int64_t len, double start, double end){
    double range_len = end - start;
    for(int64_t i = 0; i < len; ++i){
        input[i] = (rand() * range_len / RAND_MAX) + start;
    }
}

void array_copy_d(double* dst, const double* src, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void bais_d_naive(double* input, const double* bias, int64_t row, int64_t col){
    for(int64_t r = 0; r < row; ++r){
        for(int64_t c = 0; c < col; ++c){
            input[r * col + c] += bias[c];
        }
    }
}

void gelu_d_navie(double* data, int64_t len){
    const double half = 0.5;
    const double one = 1.;
    const double two = 2.;
    const double pi = M_PI;
    const double const_0 = 0.044715;
    for(int64_t i = 0; i < len; ++i){
        float x = data[i];
        data[i] = half * x * (one + tanh(sqrt(two / pi) * (x + const_0 * x * x * x)));
    }
}

void bias_gelu_d_ldm_lookup_prefetch_slave(double* input, const double* bias, int64_t row, int64_t col){
    bias_gelu_d_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_d_ldm_lookup_prefetch, &para);
    CRTS_athread_join();
}

void bias_gelu_d_ldm_lookup_prefetch_simd_slave(double* input, const double* bias, int64_t row, int64_t col){
    bias_gelu_d_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_d_ldm_lookup_prefetch_simd, &para);
    CRTS_athread_join();
}

void check_error_d(double* answer, const double* check, int64_t len){
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

void bias_glue_d_test(int64_t m, int64_t n, int64_t random_len, int64_t repeat_count){
    double* random_list = libc_aligned_malloc(random_len * sizeof(double));
    
    const int64_t M = m;
    const int64_t N = n;
    const int64_t len = M * N;

    double* bias = libc_aligned_malloc(N * sizeof(double));

    double* input  = libc_aligned_malloc(M * N * sizeof(double));
    double* output = libc_aligned_malloc(M * N * sizeof(double));
    double* answer = libc_aligned_malloc(M * N * sizeof(double));

    fill_random_d(random_list, random_len, -3, 2);

    fill_random_d(bias, N, 0., 1.);

    for(int64_t i = 0; i < len; i += random_len){
        array_copy_d(input + i, random_list, random_len);
    }

    array_copy_d(answer, input, len);

    bais_d_naive(answer, bias, M, N);
    gelu_d_navie(answer, len);
    
    double time = 0;
    for(int64_t r = 0; r < repeat_count; r++){
        array_copy_d(output, input, len);
        double time_start = MPI_Wtime();
        // bias_gelu_d_ldm_lookup_prefetch_slave(output, bias, M, N);
        bias_gelu_d_ldm_lookup_prefetch_simd_slave(output, bias, M, N);
        double time_end = MPI_Wtime();
        time += time_end - time_start;
    }

    // check_error_d(answer, output, len);

    double memory_access = 1.0 * M * N * repeat_count * sizeof(double) * 2;
    double bandwitdh = memory_access / time / 1000/ 1000/ 1000;

    printf("bias gelu kernel double precision -------------------------------------------\n");
    printf("m : %ld\n", m);
    printf("n : %ld\n", n);
    printf("repeat_count : %ld\n", repeat_count);
    printf("time : %lf\n", time);
    printf("bandwitdh : %lf GB/s\n", bandwitdh);
    printf("-----------------------------------------------------------------------------\n");

    libc_aligned_free(random_list);
    libc_aligned_free(bias);
    libc_aligned_free(input);
    libc_aligned_free(output);
    libc_aligned_free(answer);
}