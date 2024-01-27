#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void triad_naive(triad_param_t* para_p){
    triad_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(triad_param_t));

    scalar* z = para.z;
    const scalar* x = para.x;
    const scalar* y = para.y;
    const scalar alpha = para.alpha;
    const scalar beta = para.beta;
    const scalar gamma = para.gamma;
    label len = para.len;

    label local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_x = x + local_start;
    const scalar* local_y = y + local_start;
    scalar* local_z = z + local_start;

    scalar x_buffer[MAX_CELL_LOCAL];
    scalar y_buffer[MAX_CELL_LOCAL];
    scalar z_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);
    
    if(gamma != 0.){
        for(label bi = 0; bi < block_count; ++bi){
            label brs = bi * MAX_CELL_LOCAL;
            label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
            label brl = bre - brs;
            CRTS_dma_get(x_buffer, (scalar*)local_x + brs, brl * sizeof(double));
            CRTS_dma_get(y_buffer, (scalar*)local_y + brs, brl * sizeof(double));
            CRTS_dma_get(z_buffer, (scalar*)local_z + brs, brl * sizeof(double));
            for(label i = 0; i < brl; ++i){
                z_buffer[i] = alpha * x_buffer[i] + beta * y_buffer[i] + gamma * z_buffer[i];
            }
            CRTS_dma_put(local_z + brs, z_buffer, brl * sizeof(double));
        }
    }else{
        for(label bi = 0; bi < block_count; ++bi){
            label brs = bi * MAX_CELL_LOCAL;
            label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
            label brl = bre - brs;
            CRTS_dma_get(x_buffer, (scalar*)local_x + brs, brl * sizeof(double));
            CRTS_dma_get(y_buffer, (scalar*)local_y + brs, brl * sizeof(double));
            for(label i = 0; i < brl; ++i){
                z_buffer[i] = alpha * x_buffer[i] + beta * y_buffer[i];
            }
            CRTS_dma_put(local_z + brs, z_buffer, brl * sizeof(double));
        }
    }


}