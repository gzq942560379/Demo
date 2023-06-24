#!/bin/bash


export OMP_NUM_THREADS=12

mpirun -np 4 ./zgemm

