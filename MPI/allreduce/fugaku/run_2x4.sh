#!/bin/sh
#PJM -L  "node=2"
#PJM -L  "freq=2200,eco_state=2"
#PJM -L  "rscgrp=small"
#PJM -L  "elapse=00:05:00"
#PJM --mpi "max-proc-per-node=4"
#PJM -g hp230257
#PJM -s

export OMPI_MCA_plm_ple_memory_allocation_policy=localalloc
export PLE_MPI_STD_EMPTYFILE=off
export OMP_NUM_THREADS=12

mpirun ../allreduce.bin
