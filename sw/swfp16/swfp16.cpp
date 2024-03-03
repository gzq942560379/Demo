#include "swfp16.h"
#include "slave_kernel.h"

void fp16_array_from_float_array(float16* fp16, const float* fp32, uint64_t len){
    fp16_array_from_float_array_param_t para;
    para.len = len;
    para.fp32 = fp32;
    para.fp16 = fp16;

    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_array_from_float_array)), &para);
    CRTS_athread_join();

}

void fp16_array_to_float_array(float* fp32, const float16* fp16, uint64_t len){
    fp16_array_to_float_array_param_t para;
    para.len = len;
    para.fp16 = fp16;
    para.fp32 = fp32;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_array_to_float_array)), &para);
    CRTS_athread_join();
}

void fp16_array_from_double_array(float16* fp16, const double* fp64, uint64_t len){
    fp16_array_from_double_array_param_t para;
    para.len = len;
    para.fp64 = fp64;
    para.fp16 = fp16;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_array_from_double_array)), &para);
    CRTS_athread_join();

}

void fp16_array_to_double_array(double* fp64, const float16* fp16, uint64_t len){
    fp16_array_to_double_array_param_t para;
    para.len = len;
    para.fp16 = fp16;
    para.fp64 = fp64;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_array_to_double_array)), &para);
    CRTS_athread_join();
}

float16 fp16_from_float(float fp32){
    float16 ret;
    fp16_from_float_param_t para;
    para.fp16_p = &ret;
    para.fp32 = fp32;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_from_float)), &para);
    CRTS_athread_join();
    return ret;
}

float fp16_to_float(float16 fp16){
    float ret;
    fp16_to_float_param_t para;
    para.fp32_p = &ret;
    para.fp16 = fp16;
    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(fp16_to_float)), &para);
    CRTS_athread_join();
    return ret;
}