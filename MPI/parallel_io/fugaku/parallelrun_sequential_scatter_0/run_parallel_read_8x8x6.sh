#!/bin/sh
#PJM -L  "node=8"
#PJM -L  "freq=2200,eco_state=2"
#PJM -L  "rscgrp=small"
#PJM -L  "elapse=00:10:00"
#PJM --mpi "max-proc-per-node=8"
#PJM -g hp230257
#PJM -s

export OMPI_MCA_plm_ple_memory_allocation_policy=bind_local
export PLE_MPI_STD_EMPTYFILE=off
export OMP_NUM_THREADS=6

mpirun ./parallel_read_mpi