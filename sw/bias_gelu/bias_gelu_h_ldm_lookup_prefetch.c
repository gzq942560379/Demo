#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_h_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_h_ldm_lookup_prefetch(bias_gelu_h_param_t *para_p){
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
        for(int64_t c = 0; c < col; ++c){
            float16 x = row_ldm_cur[c] + bias_ldm[c];
            x = slave_max(x, range_start);
            x = slave_min(x, range_end);
            uint16_t index = (uint16_t)((x - range_start) * fit_split);
            row_ldm_cur[c] = fast_gelu_poly_table_half[index];
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(float16), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}