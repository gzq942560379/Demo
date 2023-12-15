#!/bin/bash
#PJM -L  "node=2"
#PJM -L  "freq=2200,eco_state=2"
#PJM -L  "rscgrp=small"
#PJM -L  "elapse=00:01:00"
#PJM --mpi "max-proc-per-node=8"
#PJM --llio localtmp-size=10Gi
#PJM --llio sharedtmp-size=10Gi
##PJM -x PJM_LLIO_GFSCACHE=vol0001
#PJM -g hp230257
#PJM -s

mpirun ./mpishm

