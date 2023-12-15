#!/bin/bash

# CXX=mpiFCCpx
if [ $(uname -m) == "aarch64" ]
then
    CXX=mpiFCC
else
    CXX=mpiFCCpx
fi

CXXFLAGS="-Nclang -O3 -g -std=c++11 -fopenmp"

set -ex

$CXX $CXXFLAGS ../gen_file.cpp -o gen_file
$CXX $CXXFLAGS ../parallel_read_mpi.cpp -o parallel_read_mpi
$CXX $CXXFLAGS ../parallel_read_ifstream.cpp -o parallel_read_ifstream