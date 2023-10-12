#!/bin/bash
#SBATCH --partition=a100
#SBATCH --job-name=naive
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodelist=g01
#SBATCH --exclusive

source ./env.sh

./build.sh

export LATENCY_OUTPUT_FILENAME_NAME="naive"

numactl -N 0 -m 0 ./bin/two_scale_naive
