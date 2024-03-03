#!/bin/bash

FP16_CONVERT_INC="-I/home/export/base/shisuan/jiaweile/online/guozhuoqiang/Demo/sw/swfp16"
FP16_CONVERT_LIB="/home/export/base/shisuan/jiaweile/online/guozhuoqiang/Demo/sw/swfp16/libswfp16.a"

INCS="${FP16_CONVERT_INC}"
LIBS="${FP16_CONVERT_LIB}"

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64 $INCS"

set -ex

rm -f *.bin *.o



$CC -c -mhost $CFLAGS -o bias_gelu.o ./bias_gelu.c

$CC -c -mslave $CFLAGS -o bias_gelu_d_ldm_lookup_prefetch.o ./bias_gelu_d_ldm_lookup_prefetch.c
$CC -c -mslave $CFLAGS -o bias_gelu_d_ldm_lookup_prefetch_simd.o ./bias_gelu_d_ldm_lookup_prefetch_simd.c

$CC -c -mslave $CFLAGS -o bias_gelu_s_ldm_lookup_prefetch.o ./bias_gelu_s_ldm_lookup_prefetch.c
$CC -c -mslave $CFLAGS -o bias_gelu_s_ldm_lookup_prefetch_simd.o ./bias_gelu_s_ldm_lookup_prefetch_simd.c

$CC -c -mslave $CFLAGS -o bias_gelu_h_ldm_lookup_prefetch.o ./bias_gelu_h_ldm_lookup_prefetch.c
$CC -c -mslave $CFLAGS -o bias_gelu_h_ldm_lookup_prefetch_simd.o ./bias_gelu_h_ldm_lookup_prefetch_simd.c

$CC -c -mslave $CFLAGS -o bias_gelu_h_ldm_fastexp_prefetch.o ./bias_gelu_h_ldm_fastexp_prefetch.c

$CC -mhybrid -o bias_gelu.bin *.o $LIBS -lm -lm_slave
