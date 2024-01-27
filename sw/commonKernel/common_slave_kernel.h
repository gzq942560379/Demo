#pragma once

#include <crts.h>
#include "common_slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif


extern void SLAVE_FUN(axpy_naive)(axpy_param_t* para_p);

extern void SLAVE_FUN(triad_naive)(triad_param_t* para_p);

extern void SLAVE_FUN(norm_factor_local_naive)(norm_factor_local_param_t* para_p);

extern void SLAVE_FUN(sum_naive)(sum_param_t* para_p);

extern void SLAVE_FUN(sum_mag_naive)(sum_mag_param_t* para_p);

extern void SLAVE_FUN(sum_sqr_naive)(sum_sqr_param_t* para_p);

extern void SLAVE_FUN(sum_prod_naive)(sum_prod_param_t* para_p);

extern void SLAVE_FUN(copy_naive)(copy_param_t* para_p);



#ifdef __cplusplus
}
#endif