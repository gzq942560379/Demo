#include <crts.h>
#include "slave_param.h"


void ldm_info_demo(ldm_info_demo_param_t* para_p){

    double* value = para_p->value;

    if(CRTS_tid == 0){
        printf("In ldm_info_demo : %ld -----------------------\n", CRTS_tid);
        printf("_cache_size : %ld\n", _cache_size);
        printf("_cache_size : %ld\n", _cache_size);
        printf("_ldm_share_mode : %ld\n", _ldm_share_mode);
        printf("_ldm_share_size : %ld\n", _ldm_share_size);
        printf("----------------------------------------------\n");
    }

}