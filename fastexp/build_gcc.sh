#!/bin/bash 

CXX=g++

CXXFLAGS="-O3 -fopenmp -std=c++11"

set -ex

rm -f ./fastexp

$CXX $CXXFLAGS ./fastexpd.cpp  fastexpd_test.cpp -o fastexpd_test
$CXX $CXXFLAGS ./fastexpf.cpp  fastexpf_test.cpp -o fastexpf_test
$CXX $CXXFLAGS ./fastexph.cpp  fastexph_test.cpp -o fastexph_test
