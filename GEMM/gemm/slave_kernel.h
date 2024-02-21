#pragma once

#include <crts.h>
#include "slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SLAVE_FUN(convert_float2half)(convert_float2half_param_t* para_p);

extern void SLAVE_FUN(convert_half2float)(convert_half2float_param_t* para_p);

#ifdef __cplusplus
}
#endif