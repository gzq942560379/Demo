#pragma once

#include <crts.h>
#include "slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SLAVE_FUN(bias_gelu_d_ldm_lookup_prefetch)(bias_gelu_d_param_t* para_p);
extern void SLAVE_FUN(bias_gelu_d_ldm_lookup_prefetch_simd)(bias_gelu_d_param_t* para_p);

extern void SLAVE_FUN(bias_gelu_s_ldm_lookup_prefetch)(bias_gelu_s_param_t* para_p);
extern void SLAVE_FUN(bias_gelu_s_ldm_lookup_prefetch_simd)(bias_gelu_s_param_t* para_p);

extern void SLAVE_FUN(bias_gelu_h_ldm_lookup_prefetch)(bias_gelu_h_param_t* para_p);
extern void SLAVE_FUN(bias_gelu_h_ldm_lookup_prefetch_simd)(bias_gelu_h_param_t* para_p);

extern void SLAVE_FUN(bias_gelu_h_ldm_fastexp_prefetch)(bias_gelu_h_param_t* para_p);



#ifdef __cplusplus
}
#endif