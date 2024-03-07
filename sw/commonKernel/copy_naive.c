#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void copy_naive(copy_param_t* para_p){
    copy_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(copy_param_t));

    scalar* dst = para.dst;
    const scalar* src = para.src;
    label len = para.len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_src = src + local_start;
    scalar* local_dst = dst + local_start;

    scalar buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);
    
    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(buffer, (scalar*)local_src + brs, brl * sizeof(double));
        CRTS_dma_put(local_dst + brs, buffer, brl * sizeof(double));
    }

}