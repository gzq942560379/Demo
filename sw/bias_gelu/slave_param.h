#pragma once

#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x,y) ((x) < (y) ? (x) : (y))

#define simd_expand_doublev8(x) simd_set_doublev8(x, x, x, x, x, x, x, x)

#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
    double* data;
    int64_t len;
} gelu_d_param_t;

typedef struct {
    double* input;
    const double* bias;
    int64_t row;
    int64_t col;
} bias_d_param_t;

typedef struct {
    double* input;
    const double* bias;
    int64_t row;
    int64_t col;
} bias_gelu_d_param_t;

#ifdef __cplusplus
}
#endif