#include <crts.h>
#include "slave_param.h"
#include <simd.h>
#include <stdlib.h>
#include <stdio.h>

#define TILE_SIZE 1024

void fp16_array_from_float_array(fp16_array_from_float_array_param_t* para_p){
    fp16_array_from_float_array_param_t para;
    CRTS_dma_get(&para, para_p, sizeof(fp16_array_from_float_array_param_t));

    uint64_t len = para.len;
    const float* fp32 = para.fp32;
    float16* fp16 = (float16*)para.fp16;

    uint64_t local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    uint64_t local_size = local_end - local_start;


    for(uint64_t i = local_start; i < local_end; ++i){
        fp16[i] = fp32[i];
    }


    // intv16 mask1 = simd_set_intv16(271614208, -1994362491, -208850766, 271614208, -1994362491, -208850766, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    // intv16 mask2 = simd_set_intv16(1141645376, -1572332447, 1021529132, 1687038048, -1435984279, -1091867474, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    // float fp32Buffer[TILE_SIZE];
    // float16 fp16Buffer[TILE_SIZE];

    // const float* fp32_local = fp32 + local_start;
    // float16* fp16_local = fp16 + local_start;

    // floatv8 f0,f1,f2,f3;
    // float16v32 h0, h1, h2, h3;
    // float16v32 h01, h23, h0123;
    
    // for(uint64_t bs = 0; bs < local_size; bs += TILE_SIZE){
    //     uint64_t be = slave_min(bs + TILE_SIZE, local_size);
    //     uint64_t bl = be - bs;

    //     if(bl == TILE_SIZE){
    //         CRTS_dma_get(fp32Buffer, (float*)fp32_local + bs, bl * sizeof(float));
    //         for(uint64_t i = 0; i < TILE_SIZE; i += 32){
    //             simd_load(f0, fp32Buffer + i + 0);  
    //             simd_load(f1, fp32Buffer + i + 8);  
    //             simd_load(f2, fp32Buffer + i + 16);  
    //             simd_load(f3, fp32Buffer + i + 24);
    //             h0 = simd_vfcvtsh(f0, 0);
    //             h1 = simd_vfcvtsh(f1, 0);
    //             h2 = simd_vfcvtsh(f2, 0);
    //             h3 = simd_vfcvtsh(f3, 0);
    //             h01 = simd_vshfh(h0, h1, mask1);
    //             h23 = simd_vshfh(h2, h3, mask1);
    //             h0123 = simd_vshfh(h01, h23, mask2);
    //             simd_store(h0123, fp16Buffer + i);
    //         }
    //         CRTS_dma_put(fp16_local + bs, fp16Buffer, bl * sizeof(float16));
    //     }else{
    //         const float* fp32_block = fp32_local + bs;
    //         float16* fp16_block = fp16_local + bs;
    //         for(uint64_t i = 0; i < bl; ++i){
    //             fp16_block[i] = fp32_block[i];
    //         }
    //     }
    // }

}
