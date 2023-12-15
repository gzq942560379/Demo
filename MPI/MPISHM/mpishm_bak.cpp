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

    int* shared_mem;
    int shared_element_cout = 1;

    MPI_Win win;
    MPI_Win_allocate_shared(shared_element_cout * sizeof(int), sizeof(int), MPI_INFO_NULL, shmcomm, &shared_mem, &win);

    *shared_mem = global_mpirank;

    MPI_Group global_group;
    MPI_Group shared_group;
    MPI_Comm_group (global_comm, &global_group);
    MPI_Comm_group (shmcomm, &shared_group);


    int partners[global_mpisize];
    int partners_map[global_mpisize];
    for(int p = 0; p < global_mpisize; ++p){
        partners[p] = p;
    }
    MPI_Group_translate_ranks(global_group, global_mpisize, partners, shared_group, partners_map);

    for(int p = 0; p < global_mpisize; ++p){
        std::cout << p << " : " << partners[p] << " -> " << partners_map[p] << std::endl;
    }

    int* partners_ptrs[global_mpisize];
    for (int j=0; j<global_mpisize; j++)
    {
        if (partners_map[j] != MPI_UNDEFINED){
            MPI_Aint size;
            int disp_unit;
            MPI_Win_shared_query (win, partners_map[j], &size, &disp_unit, &partners_ptrs[j]);
            std::cout << global_mpirank << " can access " << j << std::endl;
            std::cout << "*partners_ptrs[" << j << "] : " << *partners_ptrs[j] << std::endl;
            std::cout << "size : " << size << std::endl;
            std::cout << "disp_unit : " << disp_unit << std::endl;
        }else{
            std::cout << global_mpirank << " can not access " << j << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}