#include "mem.h"

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

    fprintf(latency_file, "%ld %6.2lf\n", sizeof(uint64_t) * access_region, cycles);
    fflush(latency_file);

    printf("latency_test_time : %8.2lf\n", latency_test_time);
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

void test_memory_latency_multichain(
    uint64_t access_region, 
    uint64_t access_count, 
    uint64_t repeat_count, 
    uint64_t chains, 
    FILE* latency_file)
{
    assert(access_region < RAND_MAX);

    uint64_t** ptrs;
    gen_random_access_list_multichain(&ptrs, access_region, chains);

    printf("latency_test_start ... \n");
    fflush(stdout);

    test_memory_latency_multichain_kernel(ptrs, access_region, access_count, repeat_count, chains, latency_file);

    release_random_access_list_multichain(ptrs, chains);
}

int main(){
    uint64_t access_count = env_get_int("ACCESS_COUNT", 14155776);  // data accessed larger than L3 cache size
    uint64_t sample_points = env_get_int("SAMPLE_POINTS", 16);         
    uint64_t repeat_count = env_get_int("REPEAT_COUNT", 100); 
    uint64_t access_region_start = env_get_int("ACCESS_REGION_START", 256); 
    uint64_t access_region_end = env_get_int("ACCESS_REGION_END", 268435456); 
    const char* latency_output_filename_prefix = env_get_string("LATENCY_OUTPUT_FILENAME_PREFIX", "latency"); 
    const char* latency_output_filename_suffix = env_get_string("LATENCY_OUTPUT_FILENAME_SUFFIX", ".dat"); 

#ifndef DEF_CHAIN_COUNT
    fprintf(stderr, "DEF_CHAIN_COUNT is undefined when compling\n");
    fprintf(stderr, "must define DEF_CHAIN_COUNT == chains\n");
    fflush(stderr);
    abort();
#endif

    uint64_t chains = 0;
#ifdef DEF_CHAIN_COUNT
    uint64_t chains = DEF_CHAIN_COUNT;
#endif

    printf("access_count : %ld\n", access_count);
    printf("sample_points : %ld\n", sample_points);
    printf("repeat_count : %ld\n", repeat_count);
    printf("access_region_start : %ld\n", access_region_start);
    printf("access_region_end : %ld\n", access_region_end);
    printf("chains : %ld\n", chains);
    printf("latency_output_filename_prefix : %s\n", latency_output_filename_prefix);
    printf("latency_output_filename_suffix : %s\n", latency_output_filename_suffix);
    fflush(stdout);

    char filename[128];
    sprintf(filename, "%s_%02ld%s", latency_output_filename_prefix, chains, latency_output_filename_suffix);
    FILE* latency_file = fopen(filename, "w");

    for(uint64_t access_region_pow_2 = access_region_start; access_region_pow_2 < access_region_end; access_region_pow_2 *= 2){
        uint64_t step = access_region_pow_2 / sample_points;
        for(uint64_t access_region = access_region_pow_2; access_region < min(access_region_pow_2 * 2, access_region_end); access_region += step){
            test_memory_latency_multichain(access_region, access_count, repeat_count, chains, latency_file);
        }
    }
    test_memory_latency_multichain(access_region_end, access_count, repeat_count, chains, latency_file);
    
    fclose(latency_file);

    return 0;
}