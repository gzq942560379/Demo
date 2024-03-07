#!/bin/bash

DEF="-DWM_DP -DWM_ARCH_OPTION=64"

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 $DEF"
CXX=mpicxx
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 $DEF"


set -ex

rm -f *.bin *.o

$CXX -c -mhost $CFLAGS -o main.o ./main.cpp
$CXX -c -mhost $CFLAGS -o common_kernel.o ./common_kernel.C

$CC -c -mslave $CFLAGS -o norm_factor_local_naive.o ./norm_factor_local_naive.c
$CC -c -mslave $CFLAGS -o axpy_naive.o ./axpy_naive.c
$CC -c -mslave $CFLAGS -o triad_naive.o ./triad_naive.c

$CC -c -mslave $CFLAGS -o reduce_rma.o ./reduce_rma.c
$CC -c -mslave $CFLAGS -o sum_naive.o ./sum_naive.c
$CC -c -mslave $CFLAGS -o sum_mag_naive.o ./sum_mag_naive.c
$CC -c -mslave $CFLAGS -o sum_sqr_naive.o ./sum_sqr_naive.c
$CC -c -mslave $CFLAGS -o sum_prod_naive.o ./sum_prod_naive.c
$CC -c -mslave $CFLAGS -o copy_naive.o ./copy_naive.c

$CXX -mhybrid -o main.bin *.o -lm -lm_slave
