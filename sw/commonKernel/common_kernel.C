#include "common_kernel.H"
#include "common_slave_kernel.h"
#include <crts.h>

// namespace Foam{

void axpy_navie_slave(scalar* y, scalar alpha, const scalar* x, scalar beta, label len){
    axpy_param_t para;
    para.y = y;
    para.x = x;
    para.alpha = alpha;
    para.beta = beta;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(axpy_naive)), &para);
    CRTS_athread_join();
}

void triad_naive_slave(scalar* z, scalar alpha, const scalar* x, scalar beta, const scalar* y, scalar gamma, label len){
    triad_param_t para;
    para.z = z;
    para.x = x;
    para.y = y;
    para.alpha = alpha;
    para.beta = beta;
    para.gamma = gamma;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(triad_naive)), &para);
    CRTS_athread_join();
}

void norm_factor_local_naive_slave(scalar* ret_p, const scalar* pAPtr, scalar gPsiAvg, const scalar* yAPtr, const scalar* sourcePtr, label nCells){
    norm_factor_local_param_t para;
    para.ret_p = ret_p;
    para.pAPtr = pAPtr;
    para.gPsiAvg = gPsiAvg;
    para.yAPtr = yAPtr;
    para.sourcePtr = sourcePtr;
    para.nCells = nCells;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(norm_factor_local_naive)), &para);
    CRTS_athread_join();
}

void sum_naive_slave(scalar* ret_p, const scalar* input, label len){
    sum_param_t para;
    para.ret_p = ret_p;
    para.input = input;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(sum_naive)), &para);
    CRTS_athread_join();
}

void sum_mag_naive_slave(scalar* ret_p, const scalar* input, label len){
    sum_mag_param_t para;
    para.ret_p = ret_p;
    para.input = input;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(sum_mag_naive)), &para);
    CRTS_athread_join();
}

void sum_sqr_naive_slave(scalar* ret_p, const scalar* input, label len){
    sum_sqr_param_t para;
    para.ret_p = ret_p;
    para.input = input;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(sum_sqr_naive)), &para);
    CRTS_athread_join();
}

void sum_prod_naive_slave(scalar* ret_p, const scalar* a, const scalar* b, label len){
    sum_prod_param_t para;
    para.ret_p = ret_p;
    para.a = a;
    para.b = b;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(sum_prod_naive)), &para);
    CRTS_athread_join();
}

void copy_naive_slave(scalar* dst, const scalar* src, label len){
    copy_param_t para;
    para.dst = dst;
    para.src = src;
    para.len = len;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(copy_naive)), &para);
    CRTS_athread_join();
}




// }
