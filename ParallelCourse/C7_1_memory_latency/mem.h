#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <omp.h>

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

static void release_random_access_list(uint64_t* ptr){
    free(ptr);
}


static void gen_random_access_list_multichain(uint64_t*** ptrs_p, uint64_t access_region, uint64_t chains){
    *ptrs_p = malloc(sizeof(uint64_t*) * chains);
    uint64_t** ptrs = *ptrs_p;
    for(uint64_t c = 0; c < chains; ++c){
        printf("number of chain  : %ld\n", chains);
        printf("chain number : %ld\n", c);
        fflush(stdout);
        gen_random_access_list(&ptrs[c], access_region);
    }
}

static void release_random_access_list_multichain(uint64_t** ptrs, uint64_t chains){
    for(uint64_t ci = 0; ci < chains; ++ci){
        release_random_access_list(ptrs[ci]);
    }
}