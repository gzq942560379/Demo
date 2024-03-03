#include <crts.h> 
#include <simd.h>
#include <math.h>
#include "slave_param.h"
#include "gelu_h_slave_table.h"

#define MAX_BIAS 3200

void bias_gelu_h_ldm_fastexp_prefetch(bias_gelu_h_param_t *para_p){
    float16* input = para_p->input;
    const float16* bias = para_p->bias;
    int64_t row = para_p->row;
    int64_t col = para_p->col;

    int64_t row_start = CRTS_tid * row / CRTS_MAX_SPE_NUM;
    int64_t row_end = (CRTS_tid + 1) * row / CRTS_MAX_SPE_NUM;
    int64_t row_len = row_end - row_start;

    float16 bias_ldm[MAX_BIAS];
    CRTS_dma_get(bias_ldm, (float16*)bias, col * sizeof(float16));

    float16* input_local = input + row_start * col;

    float16 row_ldm[2][MAX_BIAS];

    if(row_len > 0){
        CRTS_dma_get(row_ldm[0], input_local, col * sizeof(float16));
    }

    const float16 const_sqrt_2_div_pi = 0.7978845608028654;
    const float16 const_2 = 0.044715;
    const float16 const_half = 0.5;
    const float16 const_max = 5.5;
    const float16 const_one = 1.;
    const float16 const_two = 2.;
    const uint16_t const_sign_mask = 0x8000;
    const uint16_t const_abs_value_mask = 0x7FFF;
    const float16 const_log2e = 1.442695040;
    const float16 exp_coef[2] = { -0.05288671, 0.99232129};
    const uint16_t const_shift_half = 10;

    crts_rply_t get_reply = 0;
    crts_rply_t put_reply = 0;

    for(int64_t r = 0; r < row_len; ++r){
        int64_t cur = r & 1;
        int64_t next = (r + 1) & 1;
        float16* row_ldm_cur = row_ldm[cur];
        float16* row_ldm_next = row_ldm[next];
        float16* input_row_cur = input_local + r * col;
        float16* input_row_next = input_local + (r + 1) * col;
        CRTS_dma_wait_value(&get_reply, r);
        if(r + 1 < row_len){
            CRTS_dma_iget(row_ldm_next, input_row_next, col * sizeof(float16), &get_reply);
        }
        for(int64_t c = 0; c < col; ++c){
            float16 x = row_ldm_cur[c] + bias_ldm[c];
            float16 tanh_x = const_sqrt_2_div_pi * (x + const_2 * x * x * x);
            uint16_t tanh_x_int = *(uint16_t*)&tanh_x;
            uint16_t tanh_x_sign = tanh_x_int & const_sign_mask;
            uint16_t abs_tanh_x_int = tanh_x_int & const_abs_value_mask;
            float16 abs_tanh_x = *(float16*)&abs_tanh_x_int;
            abs_tanh_x = abs_tanh_x < const_max ? abs_tanh_x : const_max;
            float16 exp_x = const_two * abs_tanh_x;
            exp_x *= const_log2e;
            uint16_t exp_xi_int = exp_x;
            float16 exp_xi = exp_xi_int;
            float16 exp_xf = exp_x - exp_xi;
            float16 exp_k = exp_coef[1] * exp_xf + exp_coef[0] + const_one;
            uint16_t exp_e = *(uint16_t*)&exp_k;
            exp_e += (exp_xi_int << const_shift_half);
            float16 exp_ret = *(float16*)&exp_e;
            float16 abs_ret = const_one - const_two / (exp_ret + const_one);
            uint16_t tanh_ret_int = *(uint16_t*)&abs_ret | tanh_x_sign;
            float16 tanh_ret = *(float16*)&tanh_ret_int;
            row_ldm_cur[c] = const_half * x * (const_one + tanh_ret);
        }
        CRTS_dma_iput(input_row_cur, row_ldm_cur, col * sizeof(float16), &put_reply);
    }
    CRTS_dma_wait_value(&put_reply, row_len);
}