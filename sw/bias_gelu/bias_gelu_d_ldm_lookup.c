#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_d_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_d_ldm_lookup(bias_gelu_d_param_t *para_p){
    double* input = para_p->input;
    const double* bias = para_p->bias;
    int64_t row = para_p->row;
    int64_t col = para_p->col;

    int64_t row_start = CRTS_tid * row / CRTS_MAX_SPE_NUM;
    int64_t row_end = (CRTS_tid + 1) * row / CRTS_MAX_SPE_NUM;

    double bias_ldm[MAX_BIAS];
    CRTS_dma_get(&bias_ldm, (double*)bias, col * sizeof(double));

    double row_ldm[MAX_BIAS];

    for(int64_t r = row_start; r < row_end; ++r){
        double* input_row = input + r * col;
        CRTS_dma_get(row_ldm, input_row, col * sizeof(double));
        for(int64_t c = 0; c < col; ++c){
            row_ldm[c] += bias_ldm[c];
            double x = row_ldm[c];
            if(x < -5.){
                row_ldm[c] = 0.f;
            }else if(x > 5.){
                row_ldm[c] = x;
            }else{
                uint64_t index = (int)((x - range_start) * fit_split);
                double c2 = fast_gelu_poly_table_double[index][0];
                double c1 = fast_gelu_poly_table_double[index][1];
                double c0 = fast_gelu_poly_table_double[index][2];
                row_ldm[c] = ((c2 * x) + c1) * x + c0;
            }
        }
        CRTS_dma_put(input_row, row_ldm, col * sizeof(double));
    }
}