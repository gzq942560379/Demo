#pragma once

#include "slave_utils.h"

#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x,y) ((x) < (y) ? (x) : (y))

#define simd_expand_doublev8(x) simd_set_doublev8(x, x, x, x, x, x, x, x)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double* value;
} ssync_demo_param_t;

typedef struct{
    double* y;
    const double* x;
    double alpha;
    double beta;
    int64_t len;
} axpy_param_t;

typedef struct {
    double* value;
} ldm_info_demo_param_t;

typedef struct {
    double* value;
    double to_bcast;
} rma_demo_param_t;

typedef struct {
    float* value;
} fp16_demo_param_t;

#ifdef __cplusplus
}
#endif