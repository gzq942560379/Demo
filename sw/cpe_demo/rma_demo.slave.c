#include <crts.h>
#include "slave_param.h"


void rma_demo(rma_demo_param_t* para_p){

    double* value = para_p->value;
    double to_bcast;

    if(CRTS_tid == 0){
        to_bcast = para_p->to_bcast;
    }else{
        to_bcast = 0;
    }

    double to_recv = 0;

    crts_rply_t rma_rplyl = 0;
    crts_rply_t rma_rplyr = 0;
    unsigned int R_COUNTL = 0;
    unsigned int R_COUNTR = 0;

    CRTS_ssync_array();

    if(CRTS_tid == 0){
        to_recv = to_bcast;
        R_COUNTR += 63;
        CRTS_rma_wait_value(&rma_rplyr, R_COUNTR);
    }else{ // 其他线程从0get
        CRTS_rma_iget(&to_recv, &rma_rplyl, sizeof(double), 0, &to_bcast, &rma_rplyr);
        R_COUNTL += 1;
        CRTS_rma_wait_value(&rma_rplyl, R_COUNTL);
    }

    value[CRTS_tid] = to_recv;
}