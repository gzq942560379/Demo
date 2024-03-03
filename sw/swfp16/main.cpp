#include <cstdio>
#include <cstdlib>
#include <mpi.h>

#include "swfp16.h"

void array_convert_check(){
    uint64_t len = 1024 * 128 + 1;
    float* fp32_in = new float[len];
    float16* fp16 = new float16[len];
    float* fp32_out = new float[len];

    for(uint64_t i = 0; i < len; ++i){
        fp32_in[i] = i % 32768;
    }

    printf("fp16_array_from_float_array begin ...\n");
    fflush(stdout);
    
    fp16_array_from_float_array(fp16, fp32_in, len);
    
    printf("fp16_array_from_float_array end ...\n");
    fflush(stdout);
    
    printf("fp16_array_to_float_array begin ...\n");
    fflush(stdout);
    
    fp16_array_to_float_array(fp32_out, fp16, len);

    printf("fp16_array_to_float_array end ...\n");
    fflush(stdout);

    for(uint64_t i = 0; i < len; ++i){
        printf("%d : %f, %f\n", i, fp32_in[i], fp32_out[i]);
        fflush(stdout);
    }

    delete[] fp32_in;
    delete[] fp32_out;
    delete[] fp16;
}

void scalar_convert_check(){
    for(int i = 0; i < 65536; ++i){
        float answer = i;
        float after_convert = fp16_to_float(fp16_from_float(answer));
        printf("%d : %f %f\n", i, answer, after_convert);
        if(std::abs(answer - after_convert) > 1e-4){
            break;
        }
    }
}

int main(int argc,char* argv[]){
    MPI_Init(&argc, &argv);

    array_convert_check();

    MPI_Finalize();
    return 0;
}
