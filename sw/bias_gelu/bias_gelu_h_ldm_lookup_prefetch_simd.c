#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_h_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_h_ldm_lookup_prefetch_simd(bias_gelu_h_param_t *para_p){
    float16* input = para_p->input;
    const float16* bias = para_p->bias;
    int64_t row = para_p->row;
    int64_t col = para_p->col;

    int64_t row_start = CRTS_tid * row / CRTS_MAX_SPE_NUM;
    int64_t row_end = (CRTS_tid + 1) * row / CRTS_MAX_SPE_NUM;
    int64_t row_len = row_end - row_start;

    float16 bias_ldm[MAX_BIAS];
    CRTS_dma_get(bias_ldm, (float16*)bias, col * sizeof(float16));

    float16* input_local = input + row_start * col;

    float16 row_ldm[2][MAX_BIAS];

    float16 one = 1.;

    if(row_len > 0){
        CRTS_dma_get(row_ldm[0], input_local, col * sizeof(float16));
    }

    crts_rply_t get_reply = 0;
    crts_rply_t put_reply = 0;

    float16v32 vdata, vbias;
    float16v32 vtmp0, vtmp1;

    float16v32 vstart = simd_vcpyh(range_start);
    float16v32 vend = simd_vcpyh(range_end);
    float16v32 vsplit = simd_vcpyh(fit_split);

    for(int64_t r = 0; r < row_len; ++r){
        int64_t cur = r & 1;
        int64_t next = (r + 1) & 1;
        float16* row_ldm_cur = row_ldm[cur];
        float16* row_ldm_next = row_ldm[next];
        float16* input_row_cur = input_local + r * col;
        float16* input_row_next = input_local + (r + 1) * col;
        CRTS_dma_wait_value(&get_reply, r);
        if(r + 1 < row_len){
            CRTS_dma_iget(row_ldm_next, input_row_next, col * sizeof(float16), &get_reply);
        }
        for(int64_t c = 0; c < col; c += 32){
            simd_load(vdata, row_ldm_cur + c);
            simd_load(vbias, bias_ldm + c);
            vdata = simd_vaddh(vdata, vbias);
            vdata = simd_smaxh(vdata, vstart);
            vdata = simd_sminh(vdata, vend);
            vtmp0 = simd_vsubh(vdata, vstart);
            vtmp1 = simd_vmulh(vtmp0, vsplit);
            row_ldm_cur[c +  0] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  0)];
            row_ldm_cur[c +  1] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  1)];
            row_ldm_cur[c +  2] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  2)];
            row_ldm_cur[c +  3] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  3)];
            row_ldm_cur[c +  4] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  4)];
            row_ldm_cur[c +  5] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  5)];
            row_ldm_cur[c +  6] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  6)];
            row_ldm_cur[c +  7] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  7)];
            row_ldm_cur[c +  8] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  8)];
            row_ldm_cur[c +  9] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1,  9)];
            row_ldm_cur[c + 10] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 10)];
            row_ldm_cur[c + 11] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 11)];
            row_ldm_cur[c + 12] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 12)];
            row_ldm_cur[c + 13] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 13)];
            row_ldm_cur[c + 14] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 14)];
            row_ldm_cur[c + 15] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 15)];
            row_ldm_cur[c + 16] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 16)];
            row_ldm_cur[c + 17] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 17)];
            row_ldm_cur[c + 18] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 18)];
            row_ldm_cur[c + 19] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 19)];
            row_ldm_cur[c + 20] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 20)];
            row_ldm_cur[c + 21] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 21)];
            row_ldm_cur[c + 22] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 22)];
            row_ldm_cur[c + 23] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 23)];
            row_ldm_cur[c + 24] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 24)];
            row_ldm_cur[c + 25] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 25)];
            row_ldm_cur[c + 26] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 26)];
            row_ldm_cur[c + 27] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 27)];
            row_ldm_cur[c + 28] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 28)];
            row_ldm_cur[c + 29] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 29)];
            row_ldm_cur[c + 30] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 30)];
            row_ldm_cur[c + 31] = fast_gelu_poly_table_half[(uint16_t)simd_vexth(vtmp1, 31)];
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(float16), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}