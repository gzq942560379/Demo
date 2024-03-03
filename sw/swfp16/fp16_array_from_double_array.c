#include <crts.h>
#include "slave_param.h"
#include <simd.h>
#include <stdlib.h>
#include <stdio.h>

#define TILE_SIZE 1024

void fp16_array_from_double_array(fp16_array_from_double_array_param_t* para_p){
    fp16_array_from_double_array_param_t para;
    CRTS_dma_get(&para, para_p, sizeof(fp16_array_from_double_array_param_t));

    uint64_t len = para.len;
    const double* fp64 = para.fp64;
    float16* fp16 = (float16*)para.fp16;

    uint64_t local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_size = local_end - local_start;


    for(uint64_t i = local_start; i < local_end; ++i){
        fp16[i] = fp64[i];
    }
}
