#include <iostream>
#include <string>
#include <fstream>
#include <mpi.h>


int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    MPI_Comm global_comm = MPI_COMM_WORLD;
    int global_mpirank, global_mpisize;
    MPI_Comm_rank(global_comm, &global_mpirank);
    MPI_Comm_size(global_comm, &global_mpisize);

    std::cout << "global_mpirank : " << global_mpirank << std::endl;
    std::cout << "global_mpisize : " << global_mpisize << std::endl;

    MPI_Comm shmcomm;
    MPI_Comm_split_type (MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED,0, MPI_INFO_NULL, &shmcomm);

    int shmcomm_mpirank, shmcomm_mpisize;

    MPI_Comm_rank(shmcomm, &shmcomm_mpirank);
    MPI_Comm_size(shmcomm, &shmcomm_mpisize);

    std::cout << "shmcomm_mpirank : " << shmcomm_mpirank << std::endl;
    std::cout << "shmcomm_mpisize : " << shmcomm_mpisize << std::endl;


    int* local_mem;
    int shared_element_cout;

    if(shmcomm_mpirank == 0){
        shared_element_cout = 1;
    }else{
        shared_element_cout = 0;
    }

    MPI_Win win;
    MPI_Win_allocate_shared(shared_element_cout, sizeof(int), MPI_INFO_NULL, shmcomm, &local_mem, &win);

    int* share_mem;

    MPI_Aint size;
    int disp_unit;
    MPI_Win_shared_query (win, 0, &size, &disp_unit, &share_mem);

    for(int p = 0; p < shmcomm_mpisize; p++){
        if(shmcomm_mpirank == p){
            *share_mem = global_mpirank;
        }
        MPI_Barrier(shmcomm);
        std::cout << "*share_mem : " << *share_mem << std::endl;
    }



    MPI_Finalize();
    return 0;
}