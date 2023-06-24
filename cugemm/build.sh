#!/bin/bash


CXX=g++
CXXFLAGS="-O3 -std=c++11 -g -fopenmp"


module load gcc-9.5.0-gcc-4.8.5-wybfp4j
module load cuda/11.6
# module load cuda-11.6.0-gcc-9.5.0-an2g3ld

# CUDA_HOME=/share/app/cuda/cuda-11.6
CUDA_HOME=/share/app/spack/opt/spack/linux-centos7-cascadelake/gcc-9.5.0/cuda-11.6.0-an2g3ldw5aynzjnavdx5gboiig4o5z6q
CUDA_INCLUDE="-I${CUDA_HOME}/include"
CUDA_LIB="-L/share/app/magma/2.5.4/lib -lmagma -L${CUDA_HOME}/lib64 -lcublas -lcudart"


$CXX $CXXFLAGS $CUDA_INCLUDE $CUDA_LIB ./cudgemm.cpp -o cudgemm

