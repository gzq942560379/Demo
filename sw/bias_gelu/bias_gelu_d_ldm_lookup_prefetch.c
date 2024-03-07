#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_d_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_d_ldm_lookup_prefetch(bias_gelu_d_param_t *para_p){
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
        for(int64_t c = 0; c < col; ++c){
            double x = row_ldm_cur[c] + bias_ldm[c];
            x = max(x, range_start);
            x = min(x, range_end);
            uint64_t index = (int)((x - range_start) * fit_split);
            double c2 = fast_gelu_poly_table_double[index][0];
            double c1 = fast_gelu_poly_table_double[index][1];
            double c0 = fast_gelu_poly_table_double[index][2];
            row_ldm_cur[c] = ((c2 * x) + c1) * x + c0;
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(double), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}