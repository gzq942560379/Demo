#!/bin/bash -ex

CXX=mpiFCC
CXXFLAGS="-Nclang -O3 -fopenmp"

BLAS_LIB="-lfjlapackexsve"

$CXX $CXXFLAGS $BLAS_LIB dgemm.cpp -o dgemm 
$CXX $CXXFLAGS $BLAS_LIB zgemm.cpp -o zgemm 

