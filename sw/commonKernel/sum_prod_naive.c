#include "common_slave_function.h"
#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void sum_prod_naive(sum_prod_param_t* para_p){
    sum_prod_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(sum_prod_param_t));

    scalar* ret_p = para.ret_p;
    const scalar* a = para.a;
    const scalar* b = para.b;
    const label len = para.len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_a = a + local_start;
    const scalar* local_b = b + local_start;

    scalar a_buffer[MAX_CELL_LOCAL];
    scalar b_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    scalar local = 0.;

    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(a_buffer, (scalar*)local_a + brs, brl * sizeof(double));
        CRTS_dma_get(b_buffer, (scalar*)local_b + brs, brl * sizeof(double));
        for(label i = 0; i < brl; ++i){
            local += a_buffer[i] * b_buffer[i];
        }
    }

    scalar ret = reduce_rma(local);

    if(CRTS_tid == 0){
        CRTS_dma_put(ret_p, &ret, sizeof(scalar));
    }
}