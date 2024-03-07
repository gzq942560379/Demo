#pragma once
#include <crts.h>
#include "slave_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

// y += alpha * x;
typedef struct{
    scalar* y;
    const scalar* x;
    scalar alpha;
    scalar beta;
    label len;
} axpy_param_t;


typedef struct{
    scalar* z;
    const scalar* x;
    const scalar* y;
    scalar alpha;
    scalar beta;
    scalar gamma;
    label len;
} triad_param_t;


typedef struct{
    double *ret_p;
    const scalar* pAPtr;
    const scalar* yAPtr;
    const scalar* sourcePtr;
    scalar gPsiAvg;
    label nCells;
} norm_factor_local_param_t;

typedef struct{
    scalar* ret_p;
    const scalar* input;
    label len;
} sum_param_t;

typedef struct{
    double *ret_p;
    const scalar* input;
    label len;
} sum_mag_param_t;

typedef struct{
    double *ret_p;
    const scalar* input;
    label len;
} sum_sqr_param_t;

typedef struct{
    double *ret_p;
    const scalar* a;
    const scalar* b;
    label len;
} sum_prod_param_t;

typedef struct{
    scalar* dst;
    const scalar* src;
    label len;
} copy_param_t;



#ifdef __cplusplus
}
#endif