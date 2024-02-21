#pragma once

#include "slave_utils.h"

#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x,y) ((x) < (y) ? (x) : (y))

#define simd_expand_doublev8(x) simd_set_doublev8(x, x, x, x, x, x, x, x)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t len;
    const float* fp32;
    short* fp16;
} convert_float2half_param_t;

typedef struct {
    uint64_t len;
    const short* fp16;
    float* fp32;
} convert_half2float_param_t;

#ifdef __cplusplus
}

#endif
