#!/bin/bash

CXX=mpiFCCpx 
CXXFLAGS="-Nclang -O3 -m64 -march=armv8.2-a+sve -mcpu=a64fx"

$CXX $CXXFLAGS -o ../allreduce.bin ../allreduce.cpp

