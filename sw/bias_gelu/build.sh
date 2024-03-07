#!/bin/bash

CC=mpicc
CFLAGS="-msimd -O3 -mftz -mieee -faddress_align=64"

set -ex

rm -f *.bin *.o

$CC -c -mhost $CFLAGS -o bias_gelu.o ./bias_gelu.c

$CC -c -mslave $CFLAGS -o bias_d_ldm.o ./bias_d_ldm.c
$CC -c -mslave $CFLAGS -o gelu_d_ldm_lookup.o ./gelu_d_ldm_lookup.c
$CC -c -mslave $CFLAGS -o bias_gelu_d_ldm_lookup.o ./bias_gelu_d_ldm_lookup.c
$CC -c -mslave $CFLAGS -o bias_gelu_d_ldm_lookup_prefetch.o ./bias_gelu_d_ldm_lookup_prefetch.c

$CC -mhybrid -o bias_gelu.bin *.o -lm -lm_slave
