#!/bin/bash

source ./env_wuchao.sh

export OMP_NUM_THREADS=1

./dgemm |& tee dgemm.out

