#pragma once

#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x,y) ((x) < (y) ? (x) : (y))

#define simd_expand_doublev8(x) simd_set_doublev8(x, x, x, x, x, x, x, x)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double* value;
} ssync_demo_param_t;

#ifdef __cplusplus
}
#endif