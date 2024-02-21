#include <crts.h>
#include "slave_param.h"
#include <simd.h>


void fp16_demo(fp16_demo_param_t* para_p){
    float* value = para_p->value;


    float fp32[64];

    CRTS_dma_get(fp32, value, 64 * sizeof(float));

    float16 fp16[64];

    if(CRTS_tid == 0){
        printf("slave : \n");
        fflush(stdout);

        for(int i = 0; i < 64; ++i){
            printf("fp32[%d] : %f\n", i, fp32[i]);
            fflush(stdout);
        }

        printf("slave floatv8 : \n");
        fflush(stdout);
        for(int i = 0; i < 64; i += 8){
            floatv8 vv;
            simd_load(vv, fp32 + i);    
            simd_print_floatv8(vv);
            printf("\n");
            fflush(stdout);
        }

        for(int i = 0; i < 64; ++i){
            fp16[i] = fp32[i];
        }

        printf("slave float16v32 : \n");

        for(int i = 0; i < 64; i += 32){
            float16v32 vv;
            simd_load(vv, fp16 + i);    
            simd_print_float16v32(vv);
            printf("\n");
            fflush(stdout);
        }

        printf("slave 32 to 16 convert : \n");


        intv16 mask1 = simd_set_intv16(271614208, -1994362491, -208850766, 271614208, -1994362491, -208850766, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        intv16 mask2 = simd_set_intv16(1141645376, -1572332447, 1021529132, 1687038048, -1435984279, -1091867474, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        // 32 float 转 32 half
        floatv8 f0,f1,f2,f3;
        float16v32 h0, h1, h2, h3;
        simd_load(f0, fp32 + 0);  
        simd_load(f1, fp32 + 8);  
        simd_load(f2, fp32 + 16);  
        simd_load(f3, fp32 + 24);
        h0 = simd_vfcvtsh(f0, 0);
        h1 = simd_vfcvtsh(f1, 0);
        h2 = simd_vfcvtsh(f2, 0);
        h3 = simd_vfcvtsh(f3, 0);
        float16v32 h01 = simd_vshfh(h0, h1, mask1);
        float16v32 h23 = simd_vshfh(h2, h3, mask1);
        float16v32 h0123 = simd_vshfh(h01, h23, mask2);

        simd_print_float16v32(h0123);

        intv16 mask3 = simd_set_intv16(1096876544, 578970898, 1832043369, 1710443300, 1806104411, -8656901, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        
        float16v32 h0123_trans = simd_vshfh(h0123, h0123, mask3);
        f0 = simd_vfcvths(h0123_trans, 0);
        f1 = simd_vfcvths(h0123_trans, 1);
        f2 = simd_vfcvths(h0123_trans, 2);
        f3 = simd_vfcvths(h0123_trans, 3);
        simd_print_floatv8(f0);
        simd_print_floatv8(f1);
        simd_print_floatv8(f2);
        simd_print_floatv8(f3);
    }
}