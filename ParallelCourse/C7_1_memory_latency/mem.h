#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <omp.h>

#ifndef DEF_CHAIN_COUNT
#define DEF_CHAIN_COUNT 1
#endif

#if !defined(DEF_GEN_RANDOM_ACCESS_LIST) && !defined(DEF_GEN_SEQUENTIAL_ACCESS_LIST)
#define DEF_GEN_RANDOM_ACCESS_LIST
#endif

#define min(x, y) ((x) < (y) ? (x) : (y))

static int env_get_int(const char* name, int default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return atoi(tmp);
}

static const char* env_get_string(const char* name, const char* default_value){
    char* tmp = getenv(name);
    if(tmp == NULL){
        return default_value;
    }
    return tmp;
}

static inline uint64_t rdtsc(void)
{
    uint64_t msr;
    __asm__ volatile ( "rdtsc\n\t"    // Returns the time in EDX:EAX.
               "shl $32, %%rdx\n\t"  // Shift the upper bits left.
               "or %%rdx, %0"        // 'Or' in the lower bits.
               : "=a" (msr)
               :
               : "rdx");
    return msr;
}

static void gen_random_access_list(uint64_t** ptr_p, uint64_t access_region){
    printf("access_region : %ld\n", access_region);
    fflush(stdout);
    *ptr_p = malloc(sizeof(uint64_t) * access_region);
    uint64_t* ptr = *ptr_p;
    double gen_data_timer_start = omp_get_wtime();
    bool* access = malloc(sizeof(bool) * access_region);
    srand(access_region);
    for(uint64_t i = 0; i < access_region; ++i){
        ptr[i] = i;
    }
    // shuffle
    for(uint64_t i = 0; i < access_region; ++i){
        uint64_t target = rand() % access_region;
        uint64_t tmp = ptr[i];
        ptr[i] = ptr[target];
        ptr[target] = tmp;
    }
    // let cycle_size == access_region
    uint64_t cycle_size = 0;
    uint64_t gen_count = -1;
    // while(cycle_size < (uint64_t)(access_region * 0.9)){
    while(cycle_size != access_region){
        gen_count += 1;
        for(uint64_t i = 0; i < access_region; ++i){
            access[i] = false;
        }
        uint64_t pre_ptr = 0;
        for(cycle_size = 0; cycle_size < access_region; ++cycle_size){
            if(access[pre_ptr]){
                // 找一个没有被访问过的值与ptr[pre_ptr]交换位置
                uint64_t non_access_count = 0;
                for(uint64_t i = 0; i < access_region; ++i){
                    if(access[i] == false){
                        non_access_count += 1;
                    }
                }
                uint64_t target_in_non_access = rand() % non_access_count;
                uint64_t target = -1;
                uint64_t non_access_index = 0;
                for(uint64_t i = 0; i < access_region; ++i){
                    if(access[i] == false){
                        if(non_access_index == target_in_non_access){
                            target = i;
                            break;
                        }
                        non_access_index += 1;
                    }
                }
                uint64_t tmp = ptr[pre_ptr];
                ptr[pre_ptr] = ptr[target];
                ptr[target] = tmp;
                break;
            }else{
                access[pre_ptr] = true;
                pre_ptr = ptr[pre_ptr];
            }
        }
        printf("cycle_size : %ld\n", cycle_size);
        fflush(stdout);
    }
    free(access);
    double gen_data_timer_end = omp_get_wtime();
    double gen_data_timer_time = gen_data_timer_end - gen_data_timer_start;
    printf("gen count : %ld\n", gen_count);
    printf("gen data time : %8.2lf\n", gen_data_timer_time);
    fflush(stdout);
}

static void gen_sequential_access_list(uint64_t** ptr_p, uint64_t access_region){
    printf("access_region : %ld\n", access_region);
    fflush(stdout);
    *ptr_p = malloc(sizeof(uint64_t) * access_region);
    uint64_t* ptr = *ptr_p;
    double gen_data_timer_start = omp_get_wtime();
    srand(access_region);
    for(uint64_t i = 0; i < access_region; ++i){
        ptr[i] = i + i;
    }
    ptr[access_region - 1] = 0;
    double gen_data_timer_end = omp_get_wtime();
    double gen_data_timer_time = gen_data_timer_end - gen_data_timer_start;
    printf("gen data time : %8.2lf\n", gen_data_timer_time);
    fflush(stdout);
}

static void release_access_list(uint64_t* ptr){
    free(ptr);
}


static void gen_access_list_multichain(uint64_t*** ptrs_p, uint64_t access_region, uint64_t chains){
    *ptrs_p = malloc(sizeof(uint64_t*) * chains);
    uint64_t** ptrs = *ptrs_p;
    for(uint64_t c = 0; c < chains; ++c){
        printf("number of chain  : %ld\n", chains);
        printf("chain number : %ld\n", c);
        fflush(stdout);
#if defined(DEF_GEN_RANDOM_ACCESS_LIST) && defined(DEF_GEN_SEQUENTIAL_ACCESS_LIST)
        fprintf(stderr, "DEF_GEN_RANDOM_ACCESS_LIST and DEF_GEN_SEQUENTIAL_ACCESS_LIST can not defined at the same time \n");
        abort();
#endif
#ifdef DEF_GEN_RANDOM_ACCESS_LIST
        gen_random_access_list(&ptrs[c], access_region);
#endif
#ifdef DEF_GEN_SEQUENTIAL_ACCESS_LIST
        gen_sequential_access_list(&ptrs[c], access_region);
#endif
    }
}

static void release_access_list_multichain(uint64_t** ptrs, uint64_t chains){
    for(uint64_t ci = 0; ci < chains; ++ci){
        release_access_list(ptrs[ci]);
    }
}

uint64_t test_memory_latency_multichain_kernel(
    uint64_t** ptrs,
    uint64_t access_region,
    uint64_t access_count,
    uint64_t repeat_count,
    uint64_t chains,
    FILE* latency_file
){
    // warm up
#if DEF_CHAIN_COUNT > 0
    uint64_t pre_ptr_0 = 0;
    uint64_t* ptr0 = ptrs[0];
#endif
#if DEF_CHAIN_COUNT > 1
    uint64_t pre_ptr_1 = 0;
    uint64_t* ptr1 = ptrs[1];
#endif
#if DEF_CHAIN_COUNT > 2
    uint64_t pre_ptr_2 = 0;
    uint64_t* ptr2 = ptrs[2];
#endif
#if DEF_CHAIN_COUNT > 3
    uint64_t pre_ptr_3 = 0;
    uint64_t* ptr3 = ptrs[3];
#endif
#if DEF_CHAIN_COUNT > 4
    uint64_t pre_ptr_4 = 0;
    uint64_t* ptr4 = ptrs[4];
#endif
#if DEF_CHAIN_COUNT > 5
    uint64_t pre_ptr_5 = 0;
    uint64_t* ptr5 = ptrs[5];
#endif
#if DEF_CHAIN_COUNT > 6
    uint64_t pre_ptr_6 = 0;
    uint64_t* ptr6 = ptrs[6];
#endif
#if DEF_CHAIN_COUNT > 7
    uint64_t pre_ptr_7 = 0;
    uint64_t* ptr7 = ptrs[7];
#endif
#if DEF_CHAIN_COUNT > 8
    uint64_t pre_ptr_8 = 0;
    uint64_t* ptr8 = ptrs[8];
#endif
#if DEF_CHAIN_COUNT > 9
    uint64_t pre_ptr_9 = 0;
    uint64_t* ptr9 = ptrs[9];
#endif
#if DEF_CHAIN_COUNT > 10
    uint64_t pre_ptr_10 = 0;
    uint64_t* ptr10 = ptrs[10];
#endif
#if DEF_CHAIN_COUNT > 11
    uint64_t pre_ptr_11 = 0;
    uint64_t* ptr11 = ptrs[11];
#endif
#if DEF_CHAIN_COUNT > 12
    uint64_t pre_ptr_12 = 0;
    uint64_t* ptr12 = ptrs[12];
#endif
#if DEF_CHAIN_COUNT > 13
    uint64_t pre_ptr_13 = 0;
    uint64_t* ptr13 = ptrs[13];
#endif
#if DEF_CHAIN_COUNT > 14
    uint64_t pre_ptr_14 = 0;
    uint64_t* ptr14 = ptrs[14];
#endif
#if DEF_CHAIN_COUNT > 15
    uint64_t pre_ptr_15 = 0;
    uint64_t* ptr15 = ptrs[15];
#endif
    for(uint64_t i = 0; i < access_count; ++i){
#if DEF_CHAIN_COUNT > 0
        pre_ptr_0 = ptr0[pre_ptr_0];
#endif
#if DEF_CHAIN_COUNT > 1
        pre_ptr_1 = ptr1[pre_ptr_1];
#endif
#if DEF_CHAIN_COUNT > 2
        pre_ptr_2 = ptr2[pre_ptr_2];
#endif
#if DEF_CHAIN_COUNT > 3
        pre_ptr_3 = ptr3[pre_ptr_3];
#endif
#if DEF_CHAIN_COUNT > 4
        pre_ptr_4 = ptr4[pre_ptr_4];
#endif
#if DEF_CHAIN_COUNT > 5
        pre_ptr_5 = ptr5[pre_ptr_5];
#endif
#if DEF_CHAIN_COUNT > 6
        pre_ptr_6 = ptr6[pre_ptr_6];
#endif
#if DEF_CHAIN_COUNT > 7
        pre_ptr_7 = ptr7[pre_ptr_7];
#endif
#if DEF_CHAIN_COUNT > 8
        pre_ptr_8 = ptr8[pre_ptr_8];
#endif
#if DEF_CHAIN_COUNT > 9
        pre_ptr_9 = ptr9[pre_ptr_9];
#endif
#if DEF_CHAIN_COUNT > 10
        pre_ptr_10 = ptr10[pre_ptr_10];
#endif
#if DEF_CHAIN_COUNT > 11
        pre_ptr_11 = ptr11[pre_ptr_11];
#endif
#if DEF_CHAIN_COUNT > 12
        pre_ptr_12 = ptr12[pre_ptr_12];
#endif
#if DEF_CHAIN_COUNT > 13
        pre_ptr_13 = ptr13[pre_ptr_13];
#endif
#if DEF_CHAIN_COUNT > 14
        pre_ptr_14 = ptr14[pre_ptr_14];
#endif
#if DEF_CHAIN_COUNT > 15
        pre_ptr_15 = ptr15[pre_ptr_15];
#endif
    }
    double latency_test_start = omp_get_wtime();
    uint64_t cycle_start = rdtsc();

    for(int i = 0; i < repeat_count; i++){
#if DEF_CHAIN_COUNT > 0
        pre_ptr_0 = 0;
#endif
#if DEF_CHAIN_COUNT > 1
        pre_ptr_1 = 0;
#endif
#if DEF_CHAIN_COUNT > 2
        pre_ptr_2 = 0;
#endif
#if DEF_CHAIN_COUNT > 3
        pre_ptr_3 = 0;
#endif
#if DEF_CHAIN_COUNT > 4
        pre_ptr_4 = 0;
#endif
#if DEF_CHAIN_COUNT > 5
        pre_ptr_5 = 0;
#endif
#if DEF_CHAIN_COUNT > 6
        pre_ptr_6 = 0;
#endif
#if DEF_CHAIN_COUNT > 7
        pre_ptr_7 = 0;
#endif
#if DEF_CHAIN_COUNT > 8
        pre_ptr_8 = 0;
#endif
#if DEF_CHAIN_COUNT > 9
        pre_ptr_9 = 0;
#endif
#if DEF_CHAIN_COUNT > 10
        pre_ptr_10 = 0;
#endif
#if DEF_CHAIN_COUNT > 11
        pre_ptr_11 = 0;
#endif
#if DEF_CHAIN_COUNT > 12
        pre_ptr_12 = 0;
#endif
#if DEF_CHAIN_COUNT > 13
        pre_ptr_13 = 0;
#endif
#if DEF_CHAIN_COUNT > 14
        pre_ptr_14 = 0;
#endif
#if DEF_CHAIN_COUNT > 15
        pre_ptr_15 = 0;
#endif
        for(uint64_t i = 0; i < access_count; ++i){
#if DEF_CHAIN_COUNT > 0
        pre_ptr_0 = ptr0[pre_ptr_0];
#endif
#if DEF_CHAIN_COUNT > 1
        pre_ptr_1 = ptr1[pre_ptr_1];
#endif
#if DEF_CHAIN_COUNT > 2
        pre_ptr_2 = ptr2[pre_ptr_2];
#endif
#if DEF_CHAIN_COUNT > 3
        pre_ptr_3 = ptr3[pre_ptr_3];
#endif
#if DEF_CHAIN_COUNT > 4
        pre_ptr_4 = ptr4[pre_ptr_4];
#endif
#if DEF_CHAIN_COUNT > 5
        pre_ptr_5 = ptr5[pre_ptr_5];
#endif
#if DEF_CHAIN_COUNT > 6
        pre_ptr_6 = ptr6[pre_ptr_6];
#endif
#if DEF_CHAIN_COUNT > 7
        pre_ptr_7 = ptr7[pre_ptr_7];
#endif
#if DEF_CHAIN_COUNT > 8
        pre_ptr_8 = ptr8[pre_ptr_8];
#endif
#if DEF_CHAIN_COUNT > 9
        pre_ptr_9 = ptr9[pre_ptr_9];
#endif
#if DEF_CHAIN_COUNT > 10
        pre_ptr_10 = ptr10[pre_ptr_10];
#endif
#if DEF_CHAIN_COUNT > 11
        pre_ptr_11 = ptr11[pre_ptr_11];
#endif
#if DEF_CHAIN_COUNT > 12
        pre_ptr_12 = ptr12[pre_ptr_12];
#endif
#if DEF_CHAIN_COUNT > 13
        pre_ptr_13 = ptr13[pre_ptr_13];
#endif
#if DEF_CHAIN_COUNT > 14
        pre_ptr_14 = ptr14[pre_ptr_14];
#endif
#if DEF_CHAIN_COUNT > 15
        pre_ptr_15 = ptr15[pre_ptr_15];
#endif
        }
    }
    uint64_t cycle_end = rdtsc();
    double latency_test_end = omp_get_wtime();
    double latency_test_time = latency_test_end - latency_test_start;

    // cycles per load
    double cycles = (cycle_end - cycle_start) * 1.0 / access_count / repeat_count / chains;

    fprintf(latency_file, "%ld %8.4lf\n", sizeof(uint64_t) * access_region, cycles);
    fflush(latency_file);

    printf("latency_test_time : %8.4lf\n", latency_test_time);
    fflush(stdout);

    // cheat compiler
    uint64_t pre_ptr = 0;
#if DEF_CHAIN_COUNT > 0 
    pre_ptr += pre_ptr_0;
#endif
#if DEF_CHAIN_COUNT > 1 
    pre_ptr += pre_ptr_1;
#endif
#if DEF_CHAIN_COUNT > 2 
    pre_ptr += pre_ptr_2;
#endif
#if DEF_CHAIN_COUNT > 3 
    pre_ptr += pre_ptr_3;
#endif
#if DEF_CHAIN_COUNT > 4 
    pre_ptr += pre_ptr_4;
#endif
#if DEF_CHAIN_COUNT > 5 
    pre_ptr += pre_ptr_5;
#endif
#if DEF_CHAIN_COUNT > 6 
    pre_ptr += pre_ptr_6;
#endif
#if DEF_CHAIN_COUNT > 7 
    pre_ptr += pre_ptr_7;
#endif
#if DEF_CHAIN_COUNT > 8 
    pre_ptr += pre_ptr_8;
#endif
#if DEF_CHAIN_COUNT > 9 
    pre_ptr += pre_ptr_9;
#endif
#if DEF_CHAIN_COUNT > 10 
    pre_ptr += pre_ptr_10;
#endif
#if DEF_CHAIN_COUNT > 11 
    pre_ptr += pre_ptr_11;
#endif
#if DEF_CHAIN_COUNT > 12 
    pre_ptr += pre_ptr_12;
#endif
#if DEF_CHAIN_COUNT > 13 
    pre_ptr += pre_ptr_13;
#endif
#if DEF_CHAIN_COUNT > 14 
    pre_ptr += pre_ptr_14;
#endif
#if DEF_CHAIN_COUNT > 15 
    pre_ptr += pre_ptr_15;
#endif
    return pre_ptr;
}
