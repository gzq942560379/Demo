#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
    MPI_Init(&argc,&argv);
    int mpirank, mpisize;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);

    double total_start = MPI_Wtime();

    const int MAX_LEN = 32768;
    const int WARMUP = 10; 
    const int REPEAT = 100; 

    if(mpirank == 0){
        std::cout << "MPI_Allreduce benchmark" << std::endl;
        std::cout << "----------------------------------" << std::endl;
        std::cout << "mpisize : " << mpisize << std::endl;
        std::cout << "MAX_LEN : " << MAX_LEN << std::endl;
        std::cout << "WARMUP : " << WARMUP << std::endl;
        std::cout << "REPEAT : " << REPEAT << std::endl;
    }
    double* local_a = new double[MAX_LEN];
    double* global_a = new double[MAX_LEN];

    std::fill(local_a, local_a + MAX_LEN, 1);

    for(int len = 1; len <= MAX_LEN; len *= 2){
        // warm up
        for(int i = 0; i < WARMUP; ++i){
            MPI_Allreduce(local_a, global_a, len, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        }
        double time_max = 0.;
        double time_min = 1000000.0;
        double time_tot = 0.;
        double time_avg;
        for(int i = 0; i < REPEAT; ++i){
            MPI_Barrier(MPI_COMM_WORLD);
            double timer_start = MPI_Wtime();
            MPI_Allreduce(local_a, global_a, len, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            double timer_end = MPI_Wtime();
            double timer_time = timer_end - timer_start;
            time_max = std::max(timer_time ,time_max);
            time_min = std::min(timer_time ,time_min);
            time_tot += timer_time;
        }    
        time_avg = time_tot / REPEAT;
        if(mpirank == 0){
            std::cout << "----------------------------------" << std::endl;
            std::cout << "element type : double" << std::endl;
            std::cout << "element size : 8" << std::endl;
            std::cout << "element count : " << len << std::endl;
            std::cout << "avg time : " << time_avg << std::endl;
            std::cout << "min time : " << time_min << std::endl;
            std::cout << "max time : " << time_max << std::endl;
        }
    }
    double total_end = MPI_Wtime();
    double total_time = total_end - total_start;
    if(mpirank == 0){
        std::cout << "----------------------------------" << std::endl;
        std::cout << "Total time : " << total_time << std::endl;
    }
    MPI_Finalize();
    return 0;
}
