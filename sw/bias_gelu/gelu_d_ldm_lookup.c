#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "gelu_d_slave_table.h"
#include "slave_param.h"

#define GELU_d_BLOCK_SIZE (8192)

void gelu_d_ldm_lookup(gelu_d_param_t *para_p){
    
    int64_t len = para_p->len;
    double* data = para_p->data;

    int64_t start = CRTS_tid * len / CRTS_MAX_SPE_NUM;
    int64_t end = (CRTS_tid + 1) * len / CRTS_MAX_SPE_NUM;

    double data_ldm[GELU_d_BLOCK_SIZE] __attribute__ ((aligned(64)));

    for (int64_t bs = start; bs < end; bs += GELU_d_BLOCK_SIZE){
        int64_t be = min(end, bs + GELU_d_BLOCK_SIZE);
        int64_t bl = be - bs;
        CRTS_dma_get(data_ldm, data + bs, bl * sizeof(double));
        for(int64_t i = 0; i < bl; ++i){
            double x = data_ldm[i];
            if(x < -5.){
                data_ldm[i] = 0.;
            }else if(x > 5.){
                data_ldm[i] = x;
            }else{
                uint64_t index = (int)((x - range_start) * fit_split);
                double c2 = fast_gelu_poly_table_double[index][0];
                double c1 = fast_gelu_poly_table_double[index][1];
                double c0 = fast_gelu_poly_table_double[index][2];
                data_ldm[i] = ((c2 * x) + c1) * x + c0;
            }
        }
        CRTS_dma_put(data + bs, data_ldm, bl * sizeof(double));
    }
}