#!/bin/bash
set -ex

BLAS_INC="-I/usr/sw/yyzlib/hgemm/include"

BLAS_LIB="/usr/sw/yyzlib/hgemm/libswhgemm.a /usr/sw/yyzlib/xMath-SACA/libswblas.a "
# BLAS_LIB="/usr/sw/yyzlib/lapack-3.8.0/librefblas.a -lgfortran"

cd ..

rm -f *.o *.bin



CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 ${BLAS_INC}"
CXX=mpicxx
CXXFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 ${BLAS_INC}"

LINKFLAGS="${BLAS_LIB}"

$CC -c -mslave $CFLAGS -o convert_float2half.o convert_float2half.c
$CC -c -mslave $CFLAGS -o convert_half2float.o convert_half2float.c

$CXX -c -mhost $CXXFLAGS -o ./dgemm.o ./dgemm.cpp 
$CXX -mhybrid -o ./dgemm.bin ./dgemm.o $LINKFLAGS

$CXX -c -mhost $CXXFLAGS -o ./sgemm.o ./sgemm.cpp 
$CXX -mhybrid -o ./sgemm.bin ./sgemm.o $LINKFLAGS


$CXX -c -mhost $CXXFLAGS -o ./hgemm.o ./hgemm.cpp 
$CXX -mhybrid -o ./hgemm.bin ./hgemm.o convert_float2half.o convert_half2float.o $LINKFLAGS