#!/bin/bash
set -ex

cd ..

rm -f *.o *.bin

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"
CXX=mpicxx
CXXFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"


$CXX -c -mhost $CXXFLAGS -o ./allreduce.o ./allreduce.cpp
$CXX -mhybrid -o ./allreduce.bin ./allreduce.o


