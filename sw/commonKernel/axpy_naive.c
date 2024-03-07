#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void axpy_naive(axpy_param_t* para_p){
    axpy_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(axpy_param_t));

    scalar* y = para.y;
    const scalar* x = para.x;
    scalar alpha = para.alpha;
    scalar beta = para.beta;
    label len = para.len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_x = x + local_start;
    scalar* local_y = y + local_start;

    scalar y_buffer[MAX_CELL_LOCAL];
    scalar x_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(x_buffer, (scalar*)local_x + brs, brl * sizeof(double));
        CRTS_dma_get(y_buffer, local_y + brs, brl * sizeof(double));
        for(label i = 0; i < brl; ++i){
            y_buffer[i] = alpha * x_buffer[i] + beta * y_buffer[i];
        }
        CRTS_dma_put(local_y + brs, y_buffer, brl * sizeof(double));
    }

} 