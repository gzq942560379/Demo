#!/bin/bash

if [ $(uname -m) == "aarch64" ]
then
    CXX=mpiFCC
else
    CXX=mpiFCCpx
fi

CXXFLAGS="-Nclang -O3 -g -std=c++11 -fopenmp"

set -ex

$CXX $CXXFLAGS ./mpishm.cpp -o mpishm