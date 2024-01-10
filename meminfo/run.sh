#!/bin/bash

./build_fugaku.sh

mpirun -n 4 ./meminfo.bin

