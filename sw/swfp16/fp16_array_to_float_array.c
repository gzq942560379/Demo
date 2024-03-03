#include <crts.h>
#include "slave_param.h"
#include <simd.h>

#define TILE_SIZE 1024

void fp16_array_to_float_array(fp16_array_to_float_array_param_t* para_p){
    fp16_array_to_float_array_param_t para;
    CRTS_dma_get(&para, para_p, sizeof(fp16_array_to_float_array_param_t));
    uint64_t len = para.len;

    const float16* fp16 = (float16*)para.fp16;
    float* fp32 = para.fp32;

    uint64_t local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_size = local_end - local_start;

    for(uint64_t i = local_start; i < local_end; ++i){
        fp32[i] = fp16[i];
    }

    // const float16* fp16_local = fp16 + local_start;
    // float* fp32_local = fp32 + local_start;

    // float16 fp16Buffer[TILE_SIZE];
    // float fp32Buffer[TILE_SIZE];

    // floatv8 f0,f1,f2,f3;
    // float16v32 h0123, h0123T;

    // intv16 mask = simd_set_intv16(1096876544, 578970898, 1832043369, 1710443300, 1806104411, -8656901, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    
    // for(uint64_t bs = 0; bs < local_size; bs += TILE_SIZE){
    //     uint64_t be = slave_min(bs + TILE_SIZE, local_size);
    //     uint64_t bl = be - bs;

    //     if(bl == TILE_SIZE){
    //         CRTS_dma_get(fp16Buffer, (float16*)fp16_local + bs, bl * sizeof(float16));
    //         for(uint64_t i = 0; i < TILE_SIZE; i += 32){
    //             simd_load(h0123, fp16Buffer + i);  
    //             h0123T = simd_vshfh(h0123, h0123, mask);
    //             f0 = simd_vfcvths(h0123T, 0);
    //             f1 = simd_vfcvths(h0123T, 1);
    //             f2 = simd_vfcvths(h0123T, 2);
    //             f3 = simd_vfcvths(h0123T, 3);
    //             simd_store(f0, fp32Buffer + i + 0);
    //             simd_store(f1, fp32Buffer + i + 8);
    //             simd_store(f2, fp32Buffer + i + 16);
    //             simd_store(f3, fp32Buffer + i + 24);
    //         }
    //         CRTS_dma_put(fp32_local + bs, fp32Buffer, bl * sizeof(float));
    //     }else{
    //         const float16* fp16_block = fp16_local + bs;
    //         float* fp32_block = fp32_local + bs;
    //         for(uint64_t i = 0; i < bl; ++i){
    //             fp32_block[i] = fp16_block[i];
    //         }
    //     }
    // }

}
