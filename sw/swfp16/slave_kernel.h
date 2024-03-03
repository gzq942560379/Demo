#pragma once

#include "slave_param.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SLAVE_FUN(fp16_from_float)(fp16_from_float_param_t* para_p);

extern void SLAVE_FUN(fp16_to_float)(fp16_to_float_param_t* para_p);

extern void SLAVE_FUN(fp16_array_from_float_array)(fp16_array_from_float_array_param_t* para_p);

extern void SLAVE_FUN(fp16_array_to_float_array)(fp16_array_to_float_array_param_t* para_p);


extern void SLAVE_FUN(fp16_array_from_double_array)(fp16_array_from_double_array_param_t* para_p);

extern void SLAVE_FUN(fp16_array_to_double_array)(fp16_array_to_double_array_param_t* para_p);

#ifdef __cplusplus
}
#endif