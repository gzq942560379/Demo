#!/bin/sh
#PJM -L  "node=1024"
#PJM -L  "freq=2200,eco_state=2"
#PJM -L  "rscgrp=large"
#PJM -L  "elapse=00:05:00"
#PJM --mpi "max-proc-per-node=8"
#PJM --llio localtmp-size=10Gi
#PJM --llio sharedtmp-size=10Gi
##PJM -x PJM_LLIO_GFSCACHE=vol0001
#PJM -g hp230257
#PJM -s

export OMPI_MCA_plm_ple_memory_allocation_policy=bind_local
export PLE_MPI_STD_EMPTYFILE=off
export OMP_NUM_THREADS=6

time mpiexec sh -c 'if [ ${PLE_RANK_ON_NODE} -eq 0 ]; then \
                    cp ../data.dat $PJM_LOCALTMP; \
                fi'

mpirun ../parallel_read_mpi