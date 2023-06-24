#!/bin/bash

source ./env.sh

CXX=mpicxx
CXXFLAGS="-O3 -std=c++11 -g -fopenmp"

INCLUDES=
LIBS=

$CXX $CXXFLAGS MPIGPUDirect.cpp -o MPIGPUDirect