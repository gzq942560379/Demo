#!/bin/bash

# CC=mpicc
# CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"
# CXX=mpicxx
# CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"

CXX=swaCC
CFLAGS="-priv -arrayAnalyse -O3"
# CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"

set -ex

rm -f *.bin *.o

$CXX -c $CFLAGS -o main.o ./main.cpp

$CXX -mhybrid -o main.bin *.o -lm -lm_slave
