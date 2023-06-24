#!/bin/bash

source ./env_wuchao.sh

CXX=g++
CXXFLAGS="-O3 -std=c++11 -g -fopenmp"

CUDA_INCLUDE="-I${CUDA_HOME}/include"
CUDA_LIB="-L${CUDA_HOME}/lib64 -lcublas -lcudart"

$CXX $CXXFLAGS $CUDA_INCLUDE $CUDA_LIB ./cudgemm.cpp -o cudgemm
$CXX $CXXFLAGS $CUDA_INCLUDE $CUDA_LIB ./cuzgemm.cpp -o cuzgemm

