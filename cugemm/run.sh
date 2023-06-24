#!/bin/sh
#SBATCH --partition=a100
#SBATCH --job-name=cudgemm
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --nodelist=g01
#SBATCH --gres=gpu:1
#SBATCH --gpus-per-task=1

module load gcc-9.5.0-gcc-4.8.5-wybfp4j
export CUDA_HOME=/share/app/spack/opt/spack/linux-centos7-cascadelake/gcc-9.5.0/cuda-11.6.0-an2g3ldw5aynzjnavdx5gboiig4o5z6q
export CPATH="${CUDA_HOME}/include:$CPATH"
export LD_LIBRARY_PATH="/share/app/magma/2.5.4/lib:${CUDA_HOME}/lib64:$LD_LIBRARY_PATH"
# module load cuda/11.6

ldd ./cudgemm

./cudgemm |& tee cudgemm.out