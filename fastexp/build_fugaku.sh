#!/bin/bash 

if [ $(uname -m) == "aarch64" ]
then
    CXX=mpiFCC
else
    CXX=mpiFCCpx
fi

CXXFLAGS="-Nclang -O3 -fopenmp -std=c++11"

set -ex

rm -f *.bin 

$CXX $CXXFLAGS ./fastexpd.cpp  fastexpd_test.cpp -o fastexpd_test.bin
$CXX $CXXFLAGS ./fastexpf.cpp  fastexpf_test.cpp -o fastexpf_test.bin
$CXX $CXXFLAGS ./fastexph.cpp  fastexph_test.cpp -o fastexph_test.bin
