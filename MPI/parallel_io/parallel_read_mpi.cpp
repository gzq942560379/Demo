#include "common.h"

#define mpi_check(error, comm, rank) {\
    if(error != MPI_SUCCESS){\
        std::cerr << "In line " << __LINE__ << " : mpi errorr !!! in rank " << rank << std::endl;\
        MPI_Abort(comm, -1);\
    }\
}

void print_info(){
    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    if(mpirank == 0){
        std::cout << "MPI parallel io benchmark ---------------------------------" << std::endl;
        std::cout << "file size : " << file_size << " [B]" << std::endl;
        std::cout << "element count : " << element_count << std::endl;
        std::cout << "element size : " << sizeof(element_type) << std::endl;
        std::cout << "mpi size : " << mpisize << std::endl;
        std::cout << "-----------------------------------------------------------" << std::endl;
    }
}

void MPI_Init_benchmark(int *argc_p, char ***argv_p){
    double mpi_init_start = omp_get_wtime();
    MPI_Init(argc_p, argv_p);
    double mpi_init_end = omp_get_wtime();
    double mpi_init_time = mpi_init_end - mpi_init_start;

    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    print_info();

    double mpi_init_time_max;
    double mpi_init_time_min;
    double mpi_init_time_tot;
    double mpi_init_time_avg;
    MPI_Reduce(&mpi_init_time, &mpi_init_time_tot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mpi_init_time, &mpi_init_time_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mpi_init_time, &mpi_init_time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    mpi_init_time_avg = mpi_init_time_tot / mpisize;

    if(mpirank == 0){
        std::cout << "MPI Init ------------------------------------------" << std::endl;
        std::cout << "MPI Init time avg : " << mpi_init_time_avg << std::endl;
        std::cout << "MPI Init time min : " << mpi_init_time_min << std::endl;
        std::cout << "MPI Init time max : " << mpi_init_time_max << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }
}

void MPI_File_open_benchmark(MPI_File* fin_p){
    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    int error;
    int amode = MPI_MODE_RDONLY | MPI_MODE_UNIQUE_OPEN;
    double mpi_file_open_start = omp_get_wtime();
    
    error = MPI_File_open(MPI_COMM_WORLD, read_filepath.c_str(), amode, MPI_INFO_NULL, fin_p);
    
    double mpi_file_open_end = omp_get_wtime();
    double mpi_file_open_time = mpi_file_open_end - mpi_file_open_start;

    double mpi_file_open_time_max;
    double mpi_file_open_time_min;
    double mpi_file_open_time_tot;
    double mpi_file_open_time_avg;
    MPI_Reduce(&mpi_file_open_time, &mpi_file_open_time_tot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mpi_file_open_time, &mpi_file_open_time_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mpi_file_open_time, &mpi_file_open_time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    mpi_file_open_time_avg = mpi_file_open_time_tot / mpisize;

    if(mpirank == 0){
        std::cout << "MPI File Open -------------------------------------" << std::endl;
        std::cout << "MPI File open time avg : " << mpi_file_open_time_avg << std::endl;
        std::cout << "MPI File open time min : " << mpi_file_open_time_min << std::endl;
        std::cout << "MPI File open time max : " << mpi_file_open_time_max << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }

    mpi_check(error, MPI_COMM_WORLD, mpirank);
}

void MPI_parallel_read_benchmark(const MPI_File* fin_p){
    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    uint64_t local_begin = element_count * mpirank / mpisize;
    uint64_t local_end = element_count * (mpirank + 1) / mpisize;
    uint64_t local_len = local_end - local_begin;
    
    std::vector<uint64_t> data(local_len);

    int error;
    MPI_Status status;
    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_read_start = omp_get_wtime();
    
    error = MPI_File_read_at(*fin_p, local_begin * sizeof(element_type), data.data(), local_len, MPI_UINT64_T, &status);
    
    double mpi_read_end = omp_get_wtime();
    double mpi_read_time = mpi_read_end - mpi_read_start;
    MPI_Barrier(MPI_COMM_WORLD);
    double mpi_read_all_end = omp_get_wtime();
    double mpi_read_all_time = mpi_read_all_end - mpi_read_start;

    mpi_check(error, MPI_COMM_WORLD, mpirank);

    double local_bandwidth = local_len * sizeof(element_type) * 1e-9 / mpi_read_time; // GB/s

    double local_bandwidth_max;
    double local_bandwidth_min;
    double local_bandwidth_tot;
    double local_bandwidth_avg;
    MPI_Reduce(&local_bandwidth, &local_bandwidth_tot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_bandwidth, &local_bandwidth_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_bandwidth, &local_bandwidth_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    local_bandwidth_avg = local_bandwidth_tot / mpisize;

    double global_bandwidth = file_size * 1e-9 / mpi_read_all_time;

    if(mpirank == 0){
        std::cout << "Parallel read -------------------------------------" << std::endl;
        std::cout << "Local bandwidth avg : " << local_bandwidth_avg << " GB/s" << std::endl;
        std::cout << "Local bandwidth min : " << local_bandwidth_min << " GB/s" << std::endl;
        std::cout << "Local bandwidth max : " << local_bandwidth_max << " GB/s" << std::endl;
        std::cout << "Global time : " << mpi_read_all_time << std::endl;
        std::cout << "Global bandwidth : " << global_bandwidth << " GB/s" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }

    // check
    for(uint64_t i = 0; i < local_len; ++i){
        assert(data[i] == i + local_begin);
    }
}

void MPI_sequential_read_benchmark(const MPI_File* fin_p){
    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    uint64_t local_begin = element_count * mpirank / mpisize;
    uint64_t local_end = element_count * (mpirank + 1) / mpisize;
    uint64_t local_len = local_end - local_begin;
    
    std::vector<uint64_t> data(local_len);

    int error;
    MPI_Barrier(MPI_COMM_WORLD);
    double local_read_start, local_read_end, local_read_time;
    double local_sendrecv_start, local_sendrecv_end, local_sendrecv_time = 0.;

    double global_read_start = omp_get_wtime();

    // for(int p = 0; p < mpisize; ++p){
    //     if(p == mpirank){
    //         local_read_start = omp_get_wtime();
    //         error = MPI_File_read_shared(*fin_p, data.data(), local_len, MPI_UINT64_T, &status);
    //         local_read_end = omp_get_wtime();
    //         local_read_time = local_read_end - local_read_start;
    //     }
    //     MPI_Barrier(MPI_COMM_WORLD);
    // }
    MPI_Request send_request;
    MPI_Request recv_request;

    MPI_Offset offset = 0;
    for(int cur = 0; cur < mpisize; ++cur){
        int next = (cur + 1) % mpisize;
        // send new offset
        local_sendrecv_start = omp_get_wtime();

        if(mpirank == cur){
            MPI_Offset new_offset = offset + local_len * sizeof(element_type);
            MPI_Isend(&new_offset, 1, MPI_OFFSET, next, 0, MPI_COMM_WORLD, &send_request);
        }
        if(mpirank == next){
            MPI_Irecv(&offset, 1, MPI_OFFSET, cur, 0, MPI_COMM_WORLD, &recv_request);
        }
        local_sendrecv_end = omp_get_wtime();
        local_sendrecv_time += local_sendrecv_end - local_sendrecv_start;
        if(mpirank == cur){
            local_read_start = omp_get_wtime();
            MPI_Status status;
            error = MPI_File_read_at(*fin_p, offset, data.data(), local_len, MPI_UINT64_T, &status);
            // error = MPI_File_read_shared(*fin_p, data.data(), local_len, MPI_UINT64_T, &status);
            local_read_end = omp_get_wtime();
            local_read_time = local_read_end - local_read_start;
        }
        local_sendrecv_start = omp_get_wtime();
        MPI_Status status;
        if(mpirank == cur){
            MPI_Wait(&send_request, &status);
        }
        if(mpirank == next){
            MPI_Wait(&recv_request, &status);
        }
        local_sendrecv_end = omp_get_wtime();
        local_sendrecv_time += local_sendrecv_end - local_sendrecv_start;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    double global_read_end = omp_get_wtime();
    double global_read_time = global_read_end - global_read_start;

    mpi_check(error, MPI_COMM_WORLD, mpirank);

    double local_bandwidth = local_len * sizeof(element_type) * 1e-9 / local_read_time; // GB/s

    double local_bandwidth_max;
    double local_bandwidth_min;
    double local_bandwidth_tot;
    double local_bandwidth_avg;
    MPI_Reduce(&local_bandwidth, &local_bandwidth_tot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_bandwidth, &local_bandwidth_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_bandwidth, &local_bandwidth_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    local_bandwidth_avg = local_bandwidth_tot / mpisize;

    double global_bandwidth = file_size * 1e-9 / global_read_time;

    double local_sendrecv_time_max;
    double local_sendrecv_time_min;
    double local_sendrecv_time_tot;
    double local_sendrecv_time_avg;
    MPI_Reduce(&local_sendrecv_time, &local_sendrecv_time_tot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sendrecv_time, &local_sendrecv_time_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sendrecv_time, &local_sendrecv_time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    local_sendrecv_time_avg = local_sendrecv_time_tot / mpisize;


    if(mpirank == 0){
        std::cout << "Senquential read ----------------------------------" << std::endl;
        std::cout << "Local bandwidth avg : " << local_bandwidth_avg << " GB/s" << std::endl;
        std::cout << "Local bandwidth min : " << local_bandwidth_min << " GB/s" << std::endl;
        std::cout << "Local bandwidth max : " << local_bandwidth_max << " GB/s" << std::endl;
        std::cout << "Local sendrecv avg : " << local_sendrecv_time_avg << std::endl;
        std::cout << "Local sendrecv min : " << local_sendrecv_time_min << std::endl;
        std::cout << "Local sendrecv max : " << local_sendrecv_time_max << std::endl;
        std::cout << "Global time : " << global_read_time << std::endl;
        std::cout << "Global bandwidth : " << global_bandwidth << " GB/s" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }

    // check
    for(uint64_t i = 0; i < local_len; ++i){
        assert(data[i] == i + local_begin);
    }
}

void MPI_senquential_read_and_scatter_benchmark(const MPI_File* fin_p){
    int mpirank, mpisize;
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    double total_start = omp_get_wtime();
    double read_start = omp_get_wtime();
    int error;
    MPI_Status status;

    std::vector<element_type> data_send;
    if(mpirank == 0){
        data_send.resize(element_count);
        for(uint64_t bs = 0; bs < element_count; bs += mpi_read_upper_bound_elements){
            uint64_t be = std::min(element_count, bs + mpi_read_upper_bound_elements);
            uint64_t bl = be - bs;
            error = MPI_File_read_at(*fin_p, bs * sizeof(element_type), data_send.data() + bs, bl, MPI_UINT64_T, &status);
            mpi_check(error, MPI_COMM_WORLD, mpirank);
        }
    }
    double read_end = omp_get_wtime();
    double read_time = read_end - read_start;

    std::vector<int> sendcounts;
    std::vector<int> displs;

    if(mpirank == 0){
        sendcounts.resize(mpisize);
        displs.resize(mpisize);
        for(int p = 0; p < mpisize; ++p){
            int start = element_count * p / mpisize;
            int end = element_count * (p + 1) / mpisize;
            int len = end - start;
            sendcounts[p] = len;
            displs[p] = start;
        }
    }

    int recvcount;

    double scatter_start = omp_get_wtime();

    MPI_Scatter(sendcounts.data(), 1, MPI_INT, &recvcount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double scatter_end = omp_get_wtime();
    double scatter_time = scatter_end - scatter_start;


    std::vector<element_type> data_recv(recvcount);

    double scatterv_start = omp_get_wtime();

    MPI_Scatterv(data_send.data(), sendcounts.data(), displs.data(), MPI_UINT64_T,
        data_recv.data(), recvcount, MPI_UINT64_T, 0, MPI_COMM_WORLD);
    double scatterv_end = omp_get_wtime();

    double scatterv_time = scatterv_end - scatterv_start;

    double total_end = omp_get_wtime();
    double total_time = total_end - total_start;

    //check
    uint64_t local_start = element_count * mpirank / mpisize;
    uint64_t local_end = element_count * (mpirank + 1) / mpisize;
    uint64_t local_len = local_end - local_start;
    assert(recvcount == local_len);
    for(uint64_t i = 0; i < local_len; ++i){
        assert(data_recv[i] == i + local_start);
    }

    if(mpirank == 0){
        std::cout << "Senquential read and scatter benchmark ------------" << std::endl;
        std::cout << "Total time : " << total_time << std::endl;
        std::cout << "Read time : " << read_time << std::endl;
        std::cout << "Read bandwidth : " << (file_size * 1e-9 / read_time) << " GB/s" << std::endl;
        std::cout << "Scatter time : " << scatter_time << std::endl;
        std::cout << "Scatterv time : " << scatterv_time << std::endl;
        std::cout << "Global bandwidth : " << (file_size * 1e-9 / total_time) << " GB/s" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
    }
}

int main(int argc, char** argv){
    MPI_Init_benchmark(&argc, &argv);
    
    MPI_File fin;
    MPI_File_open_benchmark(&fin);

    MPI_parallel_read_benchmark(&fin);

    MPI_sequential_read_benchmark(&fin);

    MPI_senquential_read_and_scatter_benchmark(&fin);

    MPI_File_close(&fin);
    MPI_Finalize();
    return 0;
}