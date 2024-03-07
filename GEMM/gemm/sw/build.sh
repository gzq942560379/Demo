#!/bin/bash
set -ex

BLAS_INC="-I/usr/sw/yyzlib/hgemm/include"

BLAS_LIB="/usr/sw/yyzlib/hgemm/libswhgemm.a /usr/sw/yyzlib/xMath-SACA/libswblas.a "
# BLAS_LIB="/usr/sw/yyzlib/lapack-3.8.0/librefblas.a -lgfortran"

cd ..

rm -f *.o *.bin

CXX=mpicxx
CXXFLAGS="-msimd -O3 -std=c++11 -mftz -mieee -faddress_align=64 ${BLAS_INC}"
LINKFLAGS="${BLAS_LIB}"

mpicxx -c -mhost $CXXFLAGS -o ./dgemm.o ./dgemm.cpp 
mpicxx -mhybrid -o ./dgemm.bin ./dgemm.o $LINKFLAGS

mpicxx -c -mhost $CXXFLAGS -o ./sgemm.o ./sgemm.cpp 
mpicxx -mhybrid -o ./sgemm.bin ./sgemm.o $LINKFLAGS


mpicxx -c -mhost $CXXFLAGS -o ./hgemm.o ./hgemm.cpp 
mpicxx -mhybrid -o ./hgemm.bin ./hgemm.o $LINKFLAGS