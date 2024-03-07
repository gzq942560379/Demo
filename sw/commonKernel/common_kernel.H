#pragma once

#include "common_slave_kernel.h"

// namespace Foam{

// y = alpha * x + beta * y;
void axpy_navie_slave(scalar* y, scalar alpha, const scalar* x, scalar beta, label len);

// z = alpha * x + beta * y + gamma * z;
void triad_naive_slave(scalar* z, scalar alpha, const scalar* x, scalar beta, const scalar* y, scalar gamma, label len);

void copy_naive_slave(scalar* dst, const scalar* src, label len);

void norm_factor_local_naive_slave(scalar* ret_p, const scalar* pAPtr, scalar gPsiAvg, const scalar* yAPtr, const scalar* sourcePtr, label nCells);

void sum_naive_slave(scalar* ret_p, const scalar* input, label len);

void sum_mag_naive_slave(scalar* ret_p, const scalar* input, label len);

void sum_sqr_naive_slave(scalar* ret_p, const scalar* input, label len);

void sum_prod_naive_slave(scalar* ret_p, const scalar* a, const scalar* b, label len);


inline void df_axpy(scalar* y, scalar alpha, const scalar* x, scalar beta, label len){

#ifdef __sw_64__

    axpy_navie_slave(y, alpha, x, beta, len);

#else

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(label i = 0; i < len; ++i){
        y[i] = alpah * x[i] + beta * y[i];
    }

#endif

}

// c = a + alpha * b
inline void df_triad(scalar* z, scalar alpha, const scalar* x, scalar beta, const scalar* y, scalar gamma, label len){

#ifdef __sw_64__

    triad_naive_slave(z, alpha, x, beta, y, gamma, len);

#else

    for(label i = 0; i < len; ++i){
        z[i] = alpha * x[i] + beta * y[i] + gamma * z[i];
    }
    
#endif
}


inline scalar df_norm_factor_local(const scalar* pAPtr, scalar gPsiAvg, const scalar* yAPtr, const scalar* sourcePtr, label nCells){
    scalar ret;
#ifdef __sw_64__

    norm_factor_local_naive_slave(&ret, pAPtr, gPsiAvg, yAPtr, sourcePtr, nCells);

#else

    ret = 0.;
    for(label c = 0; c < nCells; ++c){
        double tmp = pAPtr[c] * gPsiAvg;
        ret += std::abs(yAPtr[c] - tmp) + std::abs(sourcePtr[c] - tmp);
    }
    
#endif

    return ret;
}

inline scalar df_sum(const scalar* input, label len){
    scalar ret;
#ifdef __sw_64__

    sum_naive_slave(&ret, input, len);

#else

    ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += input[i];
    }

#endif
    
    return ret;
}

inline scalar df_sum_mag(const scalar* input, label len){
    scalar ret;
#ifdef __sw_64__

    sum_mag_naive_slave(&ret, input, len);

#else

    ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += std::abs(input[i]);
    }
    
#endif

    return ret;
}

inline scalar df_sum_sqr(const scalar* input, label len){
    scalar ret;
#ifdef __sw_64__

    sum_sqr_naive_slave(&ret, input, len);

#else

    ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += input[i] * input[i];
    }
    
#endif

    return ret;
}

inline scalar df_sum_prod(const scalar* a, const scalar* b, label len){
    scalar ret;
#ifdef __sw_64__

    sum_prod_naive_slave(&ret, a, b, len);

#else

    ret = 0.;
    for(label i = 0; i < len; ++i){
        ret += a[i] * b[i];
    }
    
#endif

    return ret;
}


inline void df_copy(scalar* dst, const scalar* src, label len){
#ifdef __sw_64__

    copy_naive_slave(dst, src, len);

#else

    for(label i = 0; i < len; ++i){
        a[i] = b[i];
    }
    
#endif

}







// }




