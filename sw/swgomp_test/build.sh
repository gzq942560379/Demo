#!/bin/bash
set -ex

rm -f *.o *.bin *.a

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"
CXX=mpicxx
CXXFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"

$CXX -c -mhost $CXXFLAGS -o ./vfma.o ./vfma.cpp 
$CXX -mhybrid -o ./vfma.bin ./vfma.o $LIB