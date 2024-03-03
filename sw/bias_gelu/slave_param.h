#pragma once

#define slave_max(x,y) ((x) < (y) ? (y) : (x))
#define slave_min(x,y) ((x) < (y) ? (x) : (y))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __sw_host__
typedef short float16;
#endif

typedef struct {
    double* data;
    uint64_t len;
} gelu_d_param_t;

typedef struct {
    double* input;
    const double* bias;
    uint64_t row;
    uint64_t col;
} bias_d_param_t;

typedef struct {
    double* input;
    const double* bias;
    uint64_t row;
    uint64_t col;
} bias_gelu_d_param_t;

typedef struct {
    float* data;
    uint64_t len;
} gelu_s_param_t;

typedef struct {
    float* input;
    const float* bias;
    uint64_t row;
    uint64_t col;
} bias_s_param_t;

typedef struct {
    float* input;
    const float* bias;
    uint64_t row;
    uint64_t col;
} bias_gelu_s_param_t;

typedef struct {
    float16* data;
    uint64_t len;
} gelu_h_param_t;

typedef struct {
    float16* input;
    const float16* bias;
    uint64_t row;
    uint64_t col;
} bias_h_param_t;

typedef struct {
    float16* input;
    const float16* bias;
    uint64_t row;
    uint64_t col;
} bias_gelu_h_param_t;

#ifdef __cplusplus
}
#endif