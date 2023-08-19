#!/bin/bash

module load compiler/2022.0.2

CC="icc"
CFLAGS="-O1 -g -std=c11 -fopenmp"

set -ex

$CC $CFLAGS mem.c -o mem
$CC $CFLAGS -DDEF_CHAIN_COUNT=1 mem_multichain.c -o mem_multichain_1
$CC $CFLAGS -DDEF_CHAIN_COUNT=2 mem_multichain.c -o mem_multichain_2
$CC $CFLAGS -DDEF_CHAIN_COUNT=4 mem_multichain.c -o mem_multichain_4
$CC $CFLAGS -DDEF_CHAIN_COUNT=6 mem_multichain.c -o mem_multichain_6
$CC $CFLAGS -DDEF_CHAIN_COUNT=8 mem_multichain.c -o mem_multichain_8
$CC $CFLAGS -DDEF_CHAIN_COUNT=10 mem_multichain.c -o mem_multichain_10
$CC $CFLAGS -DDEF_CHAIN_COUNT=12 mem_multichain.c -o mem_multichain_12
$CC $CFLAGS -DDEF_CHAIN_COUNT=14 mem_multichain.c -o mem_multichain_14
$CC $CFLAGS -DDEF_CHAIN_COUNT=16 mem_multichain.c -o mem_multichain_16
