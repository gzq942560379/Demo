#!/bin/bash

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"
CXX=mpicxx
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"


set -ex

rm -f *.bin *.o

$CXX -c -mhost $CFLAGS -o main.o ./main.cpp

$CC -c -mslave $CFLAGS -o ssync_demo.slave.o ./ssync_demo.slave.c
$CC -c -mslave $CFLAGS -o axpy_naive.slave.o ./axpy_naive.slave.c
$CC -c -mslave $CFLAGS -o ldm_info_demo.slave.o ./ldm_info_demo.slave.c
$CC -c -mslave $CFLAGS -o rma_demo.slave.o ./rma_demo.slave.c

$CXX -mhybrid -o main.bin *.o -lm -lm_slave
