#include "bias_gelu_d.h"
#include "bias_gelu_s.h"
#include "bias_gelu_h.h"

int main(int argc, char** argv){
    
    MPI_Init(&argc, &argv);

    bias_glue_d_test(32768, 3200, 2048, 16);

    bias_glue_s_test(32768, 3200, 2048, 16);

    bias_glue_h_test(32768, 3200, 2048, 16);

    MPI_Finalize();
    
    return 0;
}