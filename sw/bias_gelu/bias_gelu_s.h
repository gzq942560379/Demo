#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <crts.h>
#include <mpi.h>
#include "slave_kernel.h"

#define SLAVE_FUN(x) slave_##x 

void fill_random_s(float* input, int64_t len, float start, float end){
    float range_len = end - start;
    for(int64_t i = 0; i < len; ++i){
        input[i] = (rand() * range_len / RAND_MAX) + start;
    }
}

void array_copy_s(float* dst, const float* src, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void bais_s_naive(float* input, const float* bias, int64_t row, int64_t col){
    for(int64_t r = 0; r < row; ++r){
        for(int64_t c = 0; c < col; ++c){
            input[r * col + c] += bias[c];
        }
    }
}

void gelu_s_navie(float* data, int64_t len){
    const float half = 0.5;
    const float one = 1.;
    const float two = 2.;
    const float pi = M_PI;
    const float const_0 = 0.044715;
    for(int64_t i = 0; i < len; ++i){
        float x = data[i];
        data[i] = half * x * (one + tanh(sqrt(two / pi) * (x + const_0 * x * x * x)));
    }
}

void bias_gelu_s_ldm_lookup_prefetch_slave(float* input, const float* bias, int64_t row, int64_t col){
    bias_gelu_s_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_s_ldm_lookup_prefetch, &para);
    CRTS_athread_join();
}

void bias_gelu_s_ldm_lookup_prefetch_simd_slave(float* input, const float* bias, int64_t row, int64_t col){
    bias_gelu_s_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_s_ldm_lookup_prefetch_simd, &para);
    CRTS_athread_join();
}

void check_error_s(float* answer, const float* check, int64_t len){
    float a = 0.;
    float b = 0.;
    float max_relative_error = 0.;
    float max_absulte_error = 0.;
    for(int64_t i = 0; i < len; ++i){
        float absulte_error = fabs(answer[i] - check[i]);
        float relative_error = absulte_error / answer[i];
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

void bias_glue_s_test(int64_t m, int64_t n, int64_t random_len, int64_t repeat_count){
    float* random_list = libc_aligned_malloc(random_len * sizeof(float));
    
    const int64_t M = m;
    const int64_t N = n;
    const int64_t len = M * N;

    float* bias = libc_aligned_malloc(N * sizeof(float));

    float* input  = libc_aligned_malloc(M * N * sizeof(float));
    float* output = libc_aligned_malloc(M * N * sizeof(float));
    float* answer = libc_aligned_malloc(M * N * sizeof(float));

    fill_random_s(random_list, random_len, -5, 4);

    fill_random_s(bias, N, 0., 1.);

    for(int64_t i = 0; i < len; i += random_len){
        array_copy_s(input + i, random_list, random_len);
    }

    array_copy_s(answer, input, len);

    bais_s_naive(answer, bias, M, N);
    gelu_s_navie(answer, len);
    
    double time = 0;
    for(int64_t r = 0; r < repeat_count; r++){
        array_copy_s(output, input, len);
        double time_start = MPI_Wtime();
        // bias_gelu_s_ldm_lookup_prefetch_slave(output, bias, M, N);
        bias_gelu_s_ldm_lookup_prefetch_simd_slave(output, bias, M, N);
        double time_end = MPI_Wtime();
        time += time_end - time_start;
    }

    // check_error_s(answer, output, len);

    double memory_access = 1.0 * M * N * repeat_count * sizeof(float) * 2;
    double bandwitdh = memory_access / time / 1000/ 1000/ 1000;

    printf("bias gelu kernel single precision -------------------------------------------\n");
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