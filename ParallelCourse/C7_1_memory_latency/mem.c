#include "mem.h"

void test_memory_latency_multichain(
    uint64_t access_region, 
    uint64_t access_count, 
    uint64_t repeat_count, 
    uint64_t chains, 
    FILE* latency_file)
{
    assert(access_region < RAND_MAX);

    uint64_t** ptrs;
    gen_access_list_multichain(&ptrs, access_region, chains);

    printf("latency_test_start ... \n");
    fflush(stdout);

    test_memory_latency_multichain_kernel(ptrs, access_region, access_count, repeat_count, chains, latency_file);

    release_access_list_multichain(ptrs, chains);
}

int main(){
    uint64_t access_count = env_get_int("ACCESS_COUNT", 14155776);  // data accessed larger than L3 cache size
    uint64_t sample_points = env_get_int("SAMPLE_POINTS", 16);         
    uint64_t repeat_count = env_get_int("REPEAT_COUNT", 100); 
    uint64_t access_region_start = env_get_int("ACCESS_REGION_START", 16); 
    uint64_t access_region_end = env_get_int("ACCESS_REGION_END", 268435456); 
    const char* latency_output_filename_dir = env_get_string("LATENCY_OUTPUT_FILENAME_DIR", "./data/"); 
    const char* latency_output_filename_prefix = env_get_string("LATENCY_OUTPUT_FILENAME_PREFIX", "latency"); 
    const char* latency_output_filename_suffix = env_get_string("LATENCY_OUTPUT_FILENAME_SUFFIX", ".dat"); 

#ifndef DEF_CHAIN_COUNT
    fprintf(stderr, "DEF_CHAIN_COUNT is undefined when compling\n");
    fprintf(stderr, "must define DEF_CHAIN_COUNT == chains\n");
    fflush(stderr);
    abort();
#endif

    uint64_t chains = DEF_CHAIN_COUNT;

    printf("access_count : %ld\n", access_count);
    printf("sample_points : %ld\n", sample_points);
    printf("repeat_count : %ld\n", repeat_count);
    printf("access_region_start : %ld\n", access_region_start);
    printf("access_region_end : %ld\n", access_region_end);
    printf("chains : %ld\n", chains);
    printf("latency_output_filename_DIR : %s\n", latency_output_filename_dir);
    printf("latency_output_filename_prefix : %s\n", latency_output_filename_prefix);
    printf("latency_output_filename_suffix : %s\n", latency_output_filename_suffix);
    fflush(stdout);

    char filename[128];
    if(chains > 1){
        sprintf(filename, "%s%s_%02ld%s", latency_output_filename_dir, latency_output_filename_prefix, chains, latency_output_filename_suffix);
    }else{
        sprintf(filename, "%s%s%s", latency_output_filename_dir, latency_output_filename_prefix, latency_output_filename_suffix);
    }
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