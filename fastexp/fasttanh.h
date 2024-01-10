#pragma once
#include "fastexp.h"

#include <arm_sve.h>
#include <algorithm>

template<typename T> T tanh_fastexp_1(T x);
template<typename T> T tanh_fastexp_2(T x);
template<typename T> T tanh_fastexp_3(T x);
template<typename T> T tanh_fastexp_4(T x);
template<typename T> T tanh_fastexp_5(T x);
template<typename T> T tanh_fastexp_6(T x);
template<typename T> T tanh_fastexp_7(T x);
template<typename T> T tanh_fastexp_8(T x);
template<typename T> T tanh_fastexp_fusion_1(T tanh_x);

template<typename T> void tanh_fastexp_simd_1(T* Y, const T* X, int len);

template<typename T>
T tanh_fastexp_1(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_1<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_2(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_2<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_3(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_3<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_4(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_4<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_5(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_5<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_6(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_6<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_7(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_7<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_8(T x){
    T sign = x < 0. ? -1. : 1.;
    T abs_value = std::abs(x);
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    T abs_return = one - two / (fastexp_8<T>(two * abs_value) + one);
    return sign * abs_return;
}

template<typename T>
T tanh_fastexp_fusion_1(T tanh_x){
    const T one = static_cast<T>(1.);
    const T two = static_cast<T>(2.);
    return one - two / (fastexp_1<T>(two * tanh_x) + one);
}

template<>
__fp16 tanh_fastexp_fusion_1<__fp16>(__fp16 tanh_x){
    const __fp16 const_max = 5.5;
    const __fp16 const_one = 1.;
    const __fp16 const_two = 2.;
    const uint16_t const_sign_mask = 0x8000;
    const uint16_t const_abs_value_mask = 0x7FFF;
    const __fp16 const_log2e = 1.442695040;
    const uint16_t const_shift = 10;

    uint16_t tanh_x_int = *(uint16_t*)&tanh_x;
    uint16_t tanh_x_sign = tanh_x_int & const_sign_mask;
    uint16_t abs_tanh_x_int = tanh_x_int & const_abs_value_mask;
    __fp16 abs_tanh_x = *(__fp16*)&abs_tanh_x_int;
    abs_tanh_x = abs_tanh_x < const_max ? abs_tanh_x : const_max;
    __fp16 exp_x = const_two * abs_tanh_x;
    exp_x *= const_log2e;
    __fp16 exp_xi = floor(exp_x);
    uint16_t exp_xi_int = exp_xi;
    __fp16 exp_xf = exp_x - exp_xi;
    const __fp16 coef[2] = { -0.05288671, 0.99232129};
    __fp16 exp_k = coef[1] * exp_xf + coef[0] + const_one;
    uint16_t exp_e = *(uint16_t*)&exp_k;
    exp_e += (exp_xi_int << const_shift);
    __fp16 exp_ret = *(__fp16*)&exp_e;
    __fp16 abs_ret = const_one - const_two / (exp_ret + const_one);
    uint16_t tanh_ret_int = *(uint16_t*)&abs_ret | tanh_x_sign;
    __fp16 tanh_ret = *(__fp16*)&tanh_ret_int;
    return tanh_ret;
}

template<typename T> 
void tanh_fastexp_simd_1(T* Y, const T* X, int len){
    for(int i = 0; i < len; ++i){
        Y[i] = tanh_fastexp_1(X[i]);
    }
}

template<> 
void tanh_fastexp_simd_1<__fp16>(__fp16* Y, const __fp16* X, int len){
    const __fp16 const_max = 5.5;
    const __fp16 const_one = 1.;
    const __fp16 const_two = 2.;
    const uint16_t const_sign_mask = 0x8000;
    const uint16_t const_abs_value_mask = 0x7FFF;
    const __fp16 const_log2e = 1.442695040;
    const uint16_t const_shift = 10;
    const __fp16 exp_coef[2] = {-0.05288671, 0.99232129};

    for(int i = 0; i < len; i += svcnth()){
        svbool_t p = svwhilelt_b16(i, len);
        svfloat16_t vtanh_x = svld1(p, &X[i]);
        svuint16_t vtanh_x_int = *(svuint16_t*)&vtanh_x;
        svuint16_t vtanh_x_sign = svand_z(p, vtanh_x_int, const_sign_mask);
        svuint16_t vabs_tanh_x_int = svand_z(p, vtanh_x_int, const_abs_value_mask);
        svfloat16_t vabs_tanh_x = *(svfloat16_t*)&vabs_tanh_x_int;
        vabs_tanh_x = svmin_z(p, vabs_tanh_x, const_max);
        svfloat16_t vexp_x = svmul_z(p, vabs_tanh_x, const_two);
        vexp_x = svmul_z(p, vexp_x, const_log2e);
        svfloat16_t vexp_xi = svrintm_z(p, vexp_x);
        svuint16_t vexp_xi_int = svcvt_u16_z(p, vexp_xi);  // convert
        svfloat16_t vexp_xf = svsub_z(p, vexp_x, vexp_xi);
        svfloat16_t vexp_k_tmp0 = svmul_z(p, vexp_xf, exp_coef[1]);
        svfloat16_t vexp_k_tmp1 = svadd_z(p, vexp_k_tmp0, exp_coef[0]);
        svfloat16_t vexp_k = svadd_z(p, vexp_k_tmp1, const_one);
        svuint16_t vexp_e = *(svuint16_t*)&vexp_k;
        svuint16_t vexp_e_tmp = svlsl_z(p, vexp_xi_int, const_shift); 
        vexp_e = svadd_z(p, vexp_e, vexp_e_tmp);
        svfloat16_t vexp_ret = *(svfloat16_t*)&vexp_e;
        svfloat16_t vabs_tanh_ret_tmp0 = svadd_z(p, vexp_ret, const_one);
        svfloat16_t vabs_tanh_ret_tmp1 = svdivr_z(p, vabs_tanh_ret_tmp0, const_two);
        svfloat16_t vabs_tanh_ret = svsubr_z(p, vabs_tanh_ret_tmp1, const_one);
        svuint16_t vabs_tanh_ret_int = *(svuint16_t*)&vabs_tanh_ret;
        svuint16_t vtanh_ret_int = svorr_z(p, vabs_tanh_ret_int, vtanh_x_sign);
        svfloat16_t vtanh_ret = *(svfloat16_t*)&vtanh_ret_int;
        svst1(p, &Y[i], vtanh_ret);
    }
}