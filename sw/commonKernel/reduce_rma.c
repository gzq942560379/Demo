#include "common_slave_function.h"

#define MAX_CELL_LOCAL 1024

scalar reduce_rma(scalar local){
    crts_rply_t rma_rplyl = 0;
    crts_rply_t rma_rplyr = 0;
    unsigned int R_COUNTL = 0;
    unsigned int R_COUNTR = 0;

    scalar ret = local;
    scalar rma_buf;

    CRTS_ssync_array();

    if(CRTS_cid % 2 == 0){
        CRTS_rma_iget(&rma_buf, &rma_rplyl, sizeof(scalar), CRTS_tid+1, &ret, &rma_rplyr);
        R_COUNTL++;
        CRTS_rma_wait_value(&rma_rplyl, R_COUNTL);
        ret += rma_buf;
    }else{
        R_COUNTR++;
        CRTS_rma_wait_value(&rma_rplyr, R_COUNTR);
    }

    if(CRTS_cid==0 || CRTS_cid==4){
        CRTS_ssync_peer(CRTS_tid+2);
        CRTS_rma_iget(&rma_buf, &rma_rplyl, sizeof(scalar), CRTS_tid+2, &ret, &rma_rplyr);
        R_COUNTL++;
        CRTS_rma_wait_value(&rma_rplyl, R_COUNTL);
        ret += rma_buf;
    }else if(CRTS_cid==2 || CRTS_cid==6){
        CRTS_ssync_peer(CRTS_tid-2);
        R_COUNTR++;
        CRTS_rma_wait_value(&rma_rplyr, R_COUNTR);
    }

    if(CRTS_cid == 0){
        CRTS_ssync_peer(CRTS_tid+4);
        CRTS_rma_iget(&rma_buf, &rma_rplyl, sizeof(scalar), CRTS_tid+4, &ret, &rma_rplyr);
        R_COUNTL++;
        CRTS_rma_wait_value(&rma_rplyl, R_COUNTL);
        ret += rma_buf;
    }else if(CRTS_cid == 4){
        CRTS_ssync_peer(CRTS_tid-4);
        R_COUNTR++;
        CRTS_rma_wait_value(&rma_rplyr, R_COUNTR);
    }

    CRTS_ssync_col();

    if(CRTS_cid == 0){
        for(uint32_t flag=1; flag<8; flag=(flag<<1)+1){
            uint32_t flag2 = flag >> 1;
            uint32_t pep = (flag2+1) * 8;
            if(!flag2 || !(CRTS_rid & flag2)){
                if(!(CRTS_rid&flag)){
                    CRTS_ssync_peer(CRTS_tid + pep);
                    CRTS_rma_iget(&rma_buf, &rma_rplyl, sizeof(scalar), CRTS_tid+pep, &ret, &rma_rplyr);
                    R_COUNTL++;
                    CRTS_rma_wait_value(&rma_rplyl, R_COUNTL);
                    ret += rma_buf;
                }else{
                    CRTS_ssync_peer(CRTS_tid - pep);
                    R_COUNTR++;
                    CRTS_rma_wait_value(&rma_rplyr, R_COUNTR);
                }
            }
        }
    }
    return ret;
}

