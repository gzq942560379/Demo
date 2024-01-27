#include <iostream>
#include <mpi.h>
#include <crts.h>

#include "slave_kernel.h"

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    CRTS_init();

    double value[64];

    ssync_demo_param_t para;
    para.value = value;

    for(int i = 0; i < 64; ++i){
        value[i] = 1.0 * i;
    }

    CRTS_athread_spawn(reinterpret_cast<void *>(SLAVE_FUN(ssync_demo)), &para);
    CRTS_athread_join();

    MPI_Finalize();
    return 0;
}