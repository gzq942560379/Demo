#!/bin/bash
#SBATCH --partition=a100
#SBATCH --job-name=mem_address_ordered_traveral
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodelist=g02
#SBATCH --exclusive

spack load numactl@2.0.14

./build.sh

# export SAMPLE_POINTS=16
# export ACCESS_REGION_START=256
# export ACCESS_REGION_END=268435456

export LATENCY_OUTPUT_FILENAME_PREFIX="cycle_address_ordered_travel"
export LATENCY_OUTPUT_FILENAME_SUFFIX=".dat"

numactl -N 0 -m 0 ./bin/mem_address_ordered_traveral
