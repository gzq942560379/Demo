#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_d_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_d_ldm_lookup_prefetch_simd(bias_gelu_d_param_t *para_p){
    double* input = para_p->input;
    const double* bias = para_p->bias;
    int64_t row = para_p->row;
    int64_t col = para_p->col;

    int64_t row_start = CRTS_tid * row / CRTS_MAX_SPE_NUM;
    int64_t row_end = (CRTS_tid + 1) * row / CRTS_MAX_SPE_NUM;
    int64_t row_len = row_end - row_start;

    double bias_ldm[MAX_BIAS];
    CRTS_dma_get(bias_ldm, (double*)bias, col * sizeof(double));

    double* input_local = input + row_start * col;

    double row_ldm[2][MAX_BIAS];

    if(row_len > 0){
        CRTS_dma_get(row_ldm[0], input_local, col * sizeof(double));
    }

    crts_rply_t get_reply = 0;
    crts_rply_t put_reply = 0;

    doublev8 vdata, vbias;
    doublev8 vtmp0, vtmp1;

    doublev8 vstart = simd_vcpyfd(range_start);
    doublev8 vend = simd_vcpyfd(range_end);
    doublev8 vsplit = simd_vcpyfd(fit_split);

    for(int64_t r = 0; r < row_len; ++r){
        int64_t cur = r & 1;
        int64_t next = (r + 1) & 1;
        double* row_ldm_cur = row_ldm[cur];
        double* row_ldm_next = row_ldm[next];
        double* input_row_cur = input_local + r * col;
        double* input_row_next = input_local + (r + 1) * col;
        CRTS_dma_wait_value(&get_reply, r);
        if(r + 1 < row_len){
            CRTS_dma_iget(row_ldm_next, input_row_next, col * sizeof(double), &get_reply);
        }
        for(int64_t c = 0; c < col; c += 8){
            simd_load(vdata, row_ldm_cur + c);
            simd_load(vbias, bias_ldm + c);
            vdata = simd_vaddd(vdata, vbias);
            vdata = simd_smaxd(vdata, vstart);
            vdata = simd_smind(vdata, vend);
            vtmp0 = simd_vsubd(vdata, vstart);
            vtmp1 = simd_vmuld(vtmp0, vsplit);
            row_ldm_cur[c +  0] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  0)];
            row_ldm_cur[c +  1] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  1)];
            row_ldm_cur[c +  2] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  2)];
            row_ldm_cur[c +  3] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  3)];
            row_ldm_cur[c +  4] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  4)];
            row_ldm_cur[c +  5] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  5)];
            row_ldm_cur[c +  6] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  6)];
            row_ldm_cur[c +  7] = fast_gelu_poly_table_double[(uint64_t)simd_vextfd(vtmp1,  7)];
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(double), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}