#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <crts.h>
#include <mpi.h>
#include "bias_gelu_s.h"
#include <swfp16.h>

typedef short float16;


void bias_gelu_h_ldm_fastexp_prefetch_slave(float16* input, const float16* bias, int64_t row, int64_t col){
    bias_gelu_h_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_h_ldm_fastexp_prefetch, &para);
    CRTS_athread_join();
}

void bias_gelu_h_ldm_lookup_prefetch_slave(float16* input, const float16* bias, int64_t row, int64_t col){
    bias_gelu_h_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_h_ldm_lookup_prefetch, &para);
    CRTS_athread_join();
}

void bias_gelu_h_ldm_lookup_prefetch_simd_slave(float16* input, const float16* bias, int64_t row, int64_t col){
    bias_gelu_h_param_t para;
    para.input = input;
    para.bias = bias;
    para.row = row;
    para.col = col;
    CRTS_athread_spawn(bias_gelu_h_ldm_lookup_prefetch_simd, &para);
    CRTS_athread_join();
}


void array_copy_h(float16* dst, const float16* src, int64_t len){
    for(int64_t i = 0; i < len; ++i){
        dst[i] = src[i];
    }
}

void fill_random_s_half_safe(float* input, int64_t len, float start, float end){
    float range_len = end - start;
    for(int64_t i = 0; i < len; ++i){
        float tmp = (rand() * range_len / RAND_MAX) + start;
        if(fabsf(tmp) < 1e-4){
            tmp = 0;
        }
        input[i] = tmp;
    }
}

void bias_glue_h_test(int64_t m, int64_t n, int64_t random_len, int64_t repeat_count){
    float* random_list_s = libc_aligned_malloc(random_len * sizeof(float));

    const int64_t M = m;
    const int64_t N = n;
    const int64_t len = M * N;

    float* bias_s = libc_aligned_malloc(N * sizeof(float));

    float* input_s  = libc_aligned_malloc(M * N * sizeof(float));
    float* output_s = libc_aligned_malloc(M * N * sizeof(float));
    float* answer_s = libc_aligned_malloc(M * N * sizeof(float));

    fill_random_s_half_safe(random_list_s, random_len, 0, 1);
    fill_random_s_half_safe(bias_s, N, 0., 1.);
    for(int64_t i = 0; i < len; i += random_len){
        array_copy_s(input_s + i, random_list_s, random_len);
    }

    array_copy_s(output_s, input_s, len);
    array_copy_s(answer_s, input_s, len);

    bais_s_naive(answer_s, bias_s, M, N);
    gelu_s_navie(answer_s, len);

    float16* bias_h = libc_aligned_malloc(N * sizeof(float16));
    float16* input_h  = libc_aligned_malloc(M * N * sizeof(float16));
    float16* output_h = libc_aligned_malloc(M * N * sizeof(float16));

    fp16_array_from_float_array(bias_h, bias_s, N);
    fp16_array_from_float_array(input_h, input_s, M * N);
    
    double time = 0;
    for(int64_t r = 0; r < repeat_count; r++){
        array_copy_h(output_h, input_h, len);
        double time_start = MPI_Wtime();
        // bias_gelu_h_ldm_lookup_prefetch_slave(output_h, bias_h, M, N);
        bias_gelu_h_ldm_lookup_prefetch_simd_slave(output_h, bias_h, M, N);
        // bias_gelu_h_ldm_fastexp_prefetch_slave(output_h, bias_h, M, N);
        double time_end = MPI_Wtime();
        time += time_end - time_start;
    }

    fp16_array_to_float_array(output_s, output_h, len);

    // check_error_s(answer_s, output_s, len);

    double memory_access = 1.0 * M * N * repeat_count * sizeof(float16) * 2;
    double bandwitdh = memory_access / time / 1000/ 1000/ 1000;

    printf("bias gelu kernel half precision -------------------------------------------\n");
    printf("m : %ld\n", m);
    printf("n : %ld\n", n);
    printf("repeat_count : %ld\n", repeat_count);
    printf("time : %lf\n", time);
    printf("bandwitdh : %lf GB/s\n", bandwitdh);
    printf("-----------------------------------------------------------------------------\n");

    libc_aligned_free(random_list_s);
    libc_aligned_free(bias_s);
    libc_aligned_free(input_s);
    libc_aligned_free(output_s);
    libc_aligned_free(answer_s);
}