#!/bin/bash

export OMP_NUM_THREADS=1
export REPEAT_COUNT=1

./bin/test

export OMP_NUM_THREADS=1
export REPEAT_COUNT=100

./bin/test

export OMP_NUM_THREADS=6
export REPEAT_COUNT=1

./bin/test

export OMP_NUM_THREADS=6
export REPEAT_COUNT=100

./bin/test