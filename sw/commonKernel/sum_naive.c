#include "common_slave_function.h"
#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void sum_naive(sum_param_t* para_p){
    scalar* ret_p = para_p->ret_p;
    const scalar* input = para_p->input;
    label len = para_p->len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_input = input + local_start;

    scalar input_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    // local reduce
    scalar local = 0;

    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(input_buffer, (scalar*)local_input + brs, brl * sizeof(double));
        for(label i = 0; i < brl; ++i){
            local += input_buffer[i];
        }
    }
    // reduce between cpe
    scalar ret = reduce_rma(local);

    // write back
    if(CRTS_tid == 0){
        CRTS_dma_put(ret_p, &ret, sizeof(scalar));
    }
}

