#!/bin/bash -ex

CXX=FCC
CXXFLAGS="-Nclang -O3 -fopenmp"

BLAS_LIB="-lfjlapackexsve"

module load compilers/kgcc/9.3.1

$CXX $CXXFLAGS $BLAS_LIB dgemm.cpp -o dgemm 
$CXX $CXXFLAGS $BLAS_LIB zgemm.cpp -o zgemm 

