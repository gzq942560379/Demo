#include "mem.h"

uint64_t test_memory_latency(uint64_t access_region, uint64_t access_count, uint64_t repeat_count, FILE* latency_file){
    assert(access_region < RAND_MAX);

    uint64_t* ptr;
    gen_random_access_list(&ptr, access_region);

    uint64_t pre_ptr = 0;
    // warm up
    for(uint64_t i = 0; i < access_count; ++i){
        pre_ptr = ptr[pre_ptr];
    }
    printf("latency_test_start ... \n");
    fflush(stdout);

    double latency_test_start = omp_get_wtime();
    uint64_t cycle_start = rdtsc();
    for(int i = 0; i < repeat_count; i++){
        pre_ptr = 0;
        for(uint64_t i = 0; i < access_count; ++i){
            pre_ptr = ptr[pre_ptr];
        }
    }
    uint64_t cycle_end = rdtsc();
    double latency_test_end = omp_get_wtime();
    double latency_test_time = latency_test_end - latency_test_start;

    double cycles = (cycle_end - cycle_start) * 1.0 / access_count / repeat_count;

    fprintf(latency_file, "%ld %6.2lf\n", sizeof(uint64_t) * access_region, cycles);
    fflush(latency_file);

    printf("latency_test_time : %8.2lf\n", latency_test_time);
    fflush(stdout);

    release_random_access_list(ptr);
    return pre_ptr;
}

int main(){
    uint64_t access_count = env_get_int("ACCESS_COUNT", 14155776);  // data accessed larger than L3 cache size
    uint64_t sample_points = env_get_int("SAMPLE_POINTS", 16);         
    uint64_t repeat_count = env_get_int("REPEAT_COUNT", 100); 
    uint64_t access_region_start = env_get_int("ACCESS_REGION_START", 256); 
    uint64_t access_region_end = env_get_int("ACCESS_REGION_END", 268435456); 
    const char* latency_output_filename = env_get_string("LATENCY_OUTPUT_FILENAME", "latency.dat"); 

    printf("access_count : %ld\n", access_count);
    printf("sample_points : %ld\n", sample_points);
    printf("repeat_count : %ld\n", repeat_count);
    printf("access_region_start : %ld\n", access_region_start);
    printf("access_region_end : %ld\n", access_region_end);
    printf("latency_output_filename : %s\n", latency_output_filename);
    fflush(stdout);

    FILE* latency_file = fopen(latency_output_filename, "w");
    for(uint64_t access_region_pow_2 = access_region_start; access_region_pow_2 < access_region_end; access_region_pow_2 *= 2){
        uint64_t step = access_region_pow_2 / sample_points;
        for(uint64_t access_region = access_region_pow_2; access_region < min(access_region_pow_2 * 2, access_region_end); access_region += step){
            test_memory_latency(access_region, access_count, repeat_count, latency_file);
        }
    }
    test_memory_latency(access_region_end, access_count, repeat_count, latency_file);
    fclose(latency_file);
    return 0;
}