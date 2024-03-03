#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_s_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_s_ldm_lookup_prefetch_simd(bias_gelu_s_param_t *para_p){
    float* input = para_p->input;
    const float* bias = para_p->bias;
    int64_t row = para_p->row;
    int64_t col = para_p->col;

    int64_t row_start = CRTS_tid * row / CRTS_MAX_SPE_NUM;
    int64_t row_end = (CRTS_tid + 1) * row / CRTS_MAX_SPE_NUM;
    int64_t row_len = row_end - row_start;

    float bias_ldm[MAX_BIAS];
    CRTS_dma_get(bias_ldm, (float*)bias, col * sizeof(float));

    float* input_local = input + row_start * col;

    float row_ldm[2][MAX_BIAS];

    if(row_len > 0){
        CRTS_dma_get(row_ldm[0], input_local, col * sizeof(float));
    }

    crts_rply_t get_reply = 0;
    crts_rply_t put_reply = 0;

    floatv8 vdata, vbias;
    floatv8 vtmp0, vtmp1;

    floatv8 vstart = simd_vcpyfs(range_start);
    floatv8 vend = simd_vcpyfs(range_end);
    floatv8 vsplit = simd_vcpyfs(fit_split);

    for(int64_t r = 0; r < row_len; ++r){
        int64_t cur = r & 1;
        int64_t next = (r + 1) & 1;
        float* row_ldm_cur = row_ldm[cur];
        float* row_ldm_next = row_ldm[next];
        float* input_row_cur = input_local + r * col;
        float* input_row_next = input_local + (r + 1) * col;
        CRTS_dma_wait_value(&get_reply, r);
        if(r + 1 < row_len){
            CRTS_dma_iget(row_ldm_next, input_row_next, col * sizeof(float), &get_reply);
        }
        for(int64_t c = 0; c < col; c += 8){
            simd_load(vdata, row_ldm_cur + c);
            simd_load(vbias, bias_ldm + c);
            vdata = simd_vadds(vdata, vbias);
            vdata = simd_smaxs(vdata, vstart);
            vdata = simd_smins(vdata, vend);
            vtmp0 = simd_vsubs(vdata, vstart);
            vtmp1 = simd_vmuls(vtmp0, vsplit);
            row_ldm_cur[c +  0] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  0)];
            row_ldm_cur[c +  1] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  1)];
            row_ldm_cur[c +  2] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  2)];
            row_ldm_cur[c +  3] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  3)];
            row_ldm_cur[c +  4] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  4)];
            row_ldm_cur[c +  5] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  5)];
            row_ldm_cur[c +  6] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  6)];
            row_ldm_cur[c +  7] = fast_gelu_poly_table_float[(uint64_t)simd_vextfs(vtmp1,  7)];
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(float), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}