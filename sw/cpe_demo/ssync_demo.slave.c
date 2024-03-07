#include <crts.h>
#include "slave_param.h"


void ssync_demo(ssync_demo_param_t* para_p){

    double* value = para_p->value;

    double value_ldm;

    if(CRTS_tid == 0){
        CRTS_dma_get(&value_ldm, value + CRTS_tid, sizeof(double));
        value_ldm += 1;
        CRTS_dma_put(value + CRTS_tid, &value_ldm, sizeof(double));
    }

    CRTS_ssync_array();  // 只能同步DMA、RMA操作，不能同步ld/st

    if(CRTS_tid == 1){
        printf("%d : %lf\n", CRTS_tid, value[0]);
    }

    if(CRTS_tid == 0){
        printf("%d : %lf\n", CRTS_tid, value[0]);
    }

}