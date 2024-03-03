#include <crts.h>
#include "slave_param.h"
#include <simd.h>

#define TILE_SIZE 1024

void fp16_from_float(fp16_from_float_param_t* para_p){
    if(CRTS_tid != 0) return;
    fp16_from_float_param_t para;
    CRTS_dma_get(&para, para_p, sizeof(fp16_from_float_param_t));
    *para.fp16_p = para.fp32;
}
