#!/bin/bash 

if [ $(uname -m) == "aarch64" ]
then
    CXX=mpiFCC
else
    CXX=mpiFCCpx
fi

CXXFLAGS="-Nclang -O3 -fopenmp -std=c++11"

set -ex

rm -f ./meminfo

$CXX $CXXFLAGS ./meminfo.cpp -o meminfo.bin
