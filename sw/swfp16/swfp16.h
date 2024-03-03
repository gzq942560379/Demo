#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __sw_host__
typedef short float16;

float16 fp16_from_float(float fp32);
float fp16_to_float(float16 fp16);

void fp16_array_from_float_array(float16* fp16, const float* fp32, uint64_t len);
void fp16_array_to_float_array(float* fp32, const float16* fp16, uint64_t len);

void fp16_array_from_double_array(float16* fp16, const double* fp64, uint64_t len);
void fp16_array_to_double_array(double* fp64, const float16* fp16, uint64_t len);

#endif

#ifdef __cplusplus
}
#endif
