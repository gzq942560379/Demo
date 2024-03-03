#!/bin/bash
set -ex

BLAS_INC="-I/usr/sw/yyzlib/hgemm/include"
BLAS_LIB="/usr/sw/yyzlib/hgemm/libswhgemm.a /usr/sw/yyzlib/xMath-SACA/libswblas.a "
# BLAS_LIB="/usr/sw/yyzlib/lapack-3.8.0/librefblas.a -lgfortran"

cd ..

rm -f *.o *.bin

INCS="${BLAS_INC}"
LIBS="${BLAS_LIB}"

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 ${INCS}"
CXX=mpicxx
CXXFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64  ${INCS}"


$CXX -c -mhost $CXXFLAGS -o ./dgemm.o ./dgemm.cpp 
$CXX -mhybrid -o ./dgemm.bin ./dgemm.o $LIBS

$CXX -c -mhost $CXXFLAGS -o ./sgemm.o ./sgemm.cpp 
$CXX -mhybrid -o ./sgemm.bin ./sgemm.o $LIBS


$CXX -c -mhost $CXXFLAGS -o ./hgemm.o ./hgemm.cpp 
$CXX -mhybrid -o ./hgemm.bin ./hgemm.o $LIBS