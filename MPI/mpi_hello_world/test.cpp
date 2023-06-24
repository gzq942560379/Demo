#include <string>
#include <vector>
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
    MPI_Init(&argc,&argv);
    int mpirank, mpisize;
    MPI_Comm_rank(MPI_COMM_WORLD,&mpirank);
    MPI_Comm_size(MPI_COMM_WORLD,&mpisize);
    std::cout << "size : " << mpisize << std::endl; 
    std::cout << "rank : " << mpirank << std::endl; 
    MPI_Finalize();
    return 0;
}