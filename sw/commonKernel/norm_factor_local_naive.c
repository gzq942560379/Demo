#include "common_slave_function.h"
#include "common_slave_param.h"

#define MAX_CELL_LOCAL 1024

void norm_factor_local_naive(norm_factor_local_param_t* para_p){
    norm_factor_local_param_t para;
    
    CRTS_dma_get(&para, para_p, sizeof(norm_factor_local_param_t));

    scalar* ret_p = para.ret_p;
    const scalar* pAPtr = para.pAPtr;
    const scalar* yAPtr = para.yAPtr;
    const scalar* sourcePtr = para.sourcePtr;
    const scalar gPsiAvg = para.gPsiAvg;
    const label nCells = para.nCells;

    label local_start = LOCAL_START(nCells, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_end = LOCAL_END(nCells, CRTS_tid, CRTS_MAX_SPE_NUM);
    label local_len = local_end - local_start;

    const scalar* local_yAPtr = yAPtr + local_start;
    const scalar* local_sourcePtr = sourcePtr + local_start;
    const scalar* local_pAPtr = pAPtr + local_start;

    scalar yAPtr_buffer[MAX_CELL_LOCAL];
    scalar sourcePtr_buffer[MAX_CELL_LOCAL];
    scalar pAPtr_buffer[MAX_CELL_LOCAL];

    label block_count = slave_div_ceil(local_len, MAX_CELL_LOCAL);

    scalar local = 0.;
    
    for(label bi = 0; bi < block_count; ++bi){
        label brs = bi * MAX_CELL_LOCAL;
        label bre = slave_min(local_len, brs + MAX_CELL_LOCAL);
        label brl = bre - brs;
        CRTS_dma_get(yAPtr_buffer, (scalar*)local_yAPtr + brs, brl * sizeof(double));
        CRTS_dma_get(sourcePtr_buffer, (scalar*)local_sourcePtr + brs, brl * sizeof(double));
        CRTS_dma_get(pAPtr_buffer, (scalar*)local_pAPtr + brs, brl * sizeof(double));
        for(label i = 0; i < brl; ++i){
            double tmp = pAPtr_buffer[i] * gPsiAvg;
            local += fabs(yAPtr_buffer[i] - tmp) + fabs(sourcePtr_buffer[i] - tmp);
        }
    }
    
    scalar ret = reduce_rma(local);

    if(CRTS_tid == 0){
        CRTS_dma_put(ret_p, &ret, sizeof(scalar));
    }
}