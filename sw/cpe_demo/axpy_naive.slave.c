#include "slave_param.h"

// #define MAX_CELL_LOCAL 1024 // 1024 * 8 / 1024 = 8KB, total : 8 * 2 = 16KB
// #define MAX_CELL_LOCAL 2048 // 2048 * 8 / 1024 = 16KB, total : 16 * 2 = 32KB
// #define MAX_CELL_LOCAL 4096 // 4096 * 8 / 1024 = 32KB, total : 32 * 2 = 64KB
// #define MAX_CELL_LOCAL 8192 // 8192 * 8 / 1024 = 64KB, total : 64 * 2 = 128KB

// #define MAX_CELL_LOCAL 12288 // 12288 * 8 / 1024 = 96KB, total : 96 * 2 = 192KB // error
// #define MAX_CELL_LOCAL 13312 // 13312 * 8 / 1024 = 104KB, total : 104 * 2 = 208KB // error
#define MAX_CELL_LOCAL 14336 // 14336 * 8 / 1024 = 112KB, total : 112 * 2 = 224KB // error

// #define MAX_CELL_LOCAL 16384 // 16384 * 8 / 1024 = 128KB, total : 128 * 2 = 256KB // error

void axpy_naive(axpy_param_t* para_p){
    axpy_param_t para;

    CRTS_dma_get(&para, para_p, sizeof(axpy_param_t));

    double* y = para.y;
    const double* x = para.x;
    double alpha = para.alpha;
    double beta = para.beta;
    int64_t len = para.len;

    int64_t local_start = LOCAL_START(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    int64_t local_end = LOCAL_END(len, CRTS_tid, CRTS_MAX_SPE_NUM);
    int64_t local_len = local_end - local_start;

    const double* local_x = x + local_start;
    double* local_y = y + local_start;

    double y_buffer[MAX_CELL_LOCAL];
    double x_buffer[MAX_CELL_LOCAL];

    int64_t block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    for(int64_t bi = 0; bi < block_count; ++bi){
        int64_t brs = bi * MAX_CELL_LOCAL;
        int64_t bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        int64_t brl = bre - brs;
        CRTS_dma_get(x_buffer, (double*)local_x + brs, brl * sizeof(double));
        CRTS_dma_get(y_buffer, local_y + brs, brl * sizeof(double));
        for(int64_t i = 0; i < brl; ++i){
            y_buffer[i] = alpha * x_buffer[i] + beta * y_buffer[i];
        }
        CRTS_dma_put(local_y + brs, y_buffer, brl * sizeof(double));
    }

} 