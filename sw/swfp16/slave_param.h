#pragma once
#include <crts.h>
#include "slave_utils.h"

#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x,y) ((x) < (y) ? (x) : (y))

#ifdef __sw_host__
typedef short float16;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float16* fp16_p;
    float fp32;
} fp16_from_float_param_t;

typedef struct {
    float* fp32_p;
    float16 fp16;
} fp16_to_float_param_t;

typedef struct {
    uint64_t len;
    const float* fp32;
    float16* fp16;
} fp16_array_from_float_array_param_t;

typedef struct {
    uint64_t len;
    const float16* fp16;
    float* fp32;
} fp16_array_to_float_array_param_t;

typedef struct {
    uint64_t len;
    const double* fp64;
    float16* fp16;
} fp16_array_from_double_array_param_t;

typedef struct {
    uint64_t len;
    const float16* fp16;
    double* fp64;
} fp16_array_to_double_array_param_t;

#ifdef __cplusplus
}

#endif
