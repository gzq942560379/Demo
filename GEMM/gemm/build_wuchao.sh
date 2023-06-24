#!/bin/bash -ex

source ./env_wuchao.sh

CXX=g++

CXXFLAGS="-O3 -g -fopenmp"

BLAS_LIB="-L/home/HPCBase/libs/openblas0.3.18_kgcc9.3.1/lib -lopenblas"

$CXX $CXXFLAGS $BLAS_LIB dgemm.cpp -o dgemm 
$CXX $CXXFLAGS $BLAS_LIB zgemm.cpp -o zgemm 

