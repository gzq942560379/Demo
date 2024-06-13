#!/bin/bash
set -ex

source /home/export/base/shisuan/jiaweile/online/guozhuoqiang/Demo/sw/swgomp-v0.2.0-1307/bin/setenv

rm -f *.o *.bin *.a

CC=mpixcc.py
CFLAGS="-save-temps -msimd -O3 -mftz -mieee -faddress_align=64"
CXX=mpixcxx.py
CXXFLAGS="-save-temps -msimd -O3 -mftz -mieee -faddress_align=64"

$CXX -c $CXXFLAGS -o ./vfma.o ./vfma.cpp 
$CXX -mhybrid -o ./vfma.bin ./vfma.o $LIB

mpixcxx.py -h