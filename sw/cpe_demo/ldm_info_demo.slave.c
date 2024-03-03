#include <crts.h>
#include "slave_param.h"


void ldm_info_demo(ldm_info_demo_param_t* para_p){

    double* value = para_p->value;

    if(CRTS_tid == 0){
        printf("In ldm_info_demo : %ld -----------------------\n", CRTS_tid);
        printf("CRTS_rank : %ld\n", CRTS_rank);
        printf("CRTS_size : %ld\n", CRTS_size);
        printf("CRTS_node_rank : %ld\n", CRTS_node_rank);
        printf("CRTS_node_size : %ld\n", CRTS_node_size);
        printf("CRTS_jobid : %ld\n", CRTS_jobid);
        printf("_cache_size : %ld\n", _cache_size);
        printf("_cache_size : %ld\n", _cache_size);
        printf("_ldm_share_mode : %ld\n", _ldm_share_mode);
        printf("_ldm_share_size : %ld\n", _ldm_share_size);
        printf("----------------------------------------------\n");

    }

}