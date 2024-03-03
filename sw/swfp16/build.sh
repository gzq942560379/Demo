#!/bin/bash
set -ex

rm -f *.o *.bin *.a

LIB=libswfp16.a

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"
CXX=mpicxx
CXXFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"

AR=swar
ARFLAGS="cr"

RANLIB=swranlib
	

$CC -c -mslave $CFLAGS -o fp16_array_from_float_array.o fp16_array_from_float_array.c
$CC -c -mslave $CFLAGS -o fp16_array_to_float_array.o fp16_array_to_float_array.c

$CC -c -mslave $CFLAGS -o fp16_array_from_double_array.o fp16_array_from_double_array.c
$CC -c -mslave $CFLAGS -o fp16_array_to_double_array.o fp16_array_to_double_array.c

$CC -c -mslave $CFLAGS -o fp16_from_float.o fp16_from_float.c
$CC -c -mslave $CFLAGS -o fp16_to_float.o fp16_to_float.c

$CXX -c -mhost $CXXFLAGS -o swfp16.o ./swfp16.cpp 

# $AR $ARFLAGS $LIB swfp16.o fp16_array_to_float_array.o fp16_array_from_float_array.o fp16_from_float.o fp16_to_float.o
$AR $ARFLAGS $LIB *.o
$RANLIB $LIB

$CXX -c -mhost $CXXFLAGS -o ./main.o ./main.cpp 
$CXX -mhybrid -o ./main.bin ./main.o $LIB