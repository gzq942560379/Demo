#pragma once

#include <crts.h>
#include "slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SLAVE_FUN(gelu_d_ldm_lookup)(gelu_d_param_t* para_p);
extern void SLAVE_FUN(bias_d_ldm)(bias_d_param_t* para_p);
extern void SLAVE_FUN(bias_gelu_d_ldm_lookup)(bias_d_param_t* para_p);
extern void SLAVE_FUN(bias_gelu_d_ldm_lookup_prefetch)(bias_d_param_t* para_p);

#ifdef __cplusplus
}
#endif