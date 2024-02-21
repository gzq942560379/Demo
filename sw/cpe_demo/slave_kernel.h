#pragma once

#include <crts.h>
#include "slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SLAVE_FUN(ldm_info_demo)(ldm_info_demo_param_t* para_p);

extern void SLAVE_FUN(ssync_demo)(ssync_demo_param_t* para_p);

extern void SLAVE_FUN(axpy_naive)(axpy_param_t* para_p);

extern void SLAVE_FUN(rma_demo)(rma_demo_param_t* para_p);

extern void SLAVE_FUN(fp16_demo)(fp16_demo_param_t* para_p);


#ifdef __cplusplus
}
#endif