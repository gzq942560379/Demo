#!/bin/bash
#SBATCH --partition=a100
#SBATCH --job-name=local
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodelist=g02
#SBATCH --exclusive

spack load numactl@2.0.14
module load likwid/5.2.2

./build.sh

likwid-setFrequencies -g performance

export SAMPLE_POINTS=16
export ACCESS_REGION_START=256
export ACCESS_REGION_END=268435456
export LATENCY_OUTPUT_FILENAME="latency_local.dat"

numactl -N 0 -m 0 ./mem
