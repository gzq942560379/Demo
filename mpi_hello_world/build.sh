#!/bin/bash

source ./env.sh

CXXFLAGS="-O2 -g -std=c++11"
mpicxx $CXXFLAGS -o test ./test.cpp 
