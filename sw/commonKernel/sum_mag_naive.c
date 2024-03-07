#include "common_slave_function.h"
#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void sum_mag_naive(sum_mag_param_t* para_p){
    sum_mag_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(sum_mag_param_t));

    scalar* ret_p = para.ret_p;
    const scalar* input = para.input;
    const label len = para.len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_input = input + local_start;

    scalar input_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    scalar local = 0.;

    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(input_buffer, (scalar*)local_input + brs, brl * sizeof(double));
        for(label i = 0; i < brl; ++i){
            local += fabs(input_buffer[i]);
        }
    }

    scalar ret = reduce_rma(local);

    if(CRTS_tid == 0){
        CRTS_dma_put(ret_p, &ret, sizeof(scalar));
    }
}