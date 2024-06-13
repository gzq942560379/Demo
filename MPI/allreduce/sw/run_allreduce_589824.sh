#!/bin/bash

proc=589824

Timestamp=$(date "+%Y%m%d%H%M%S")


name=allreduce_${proc}
output_file=${name}.out.${Timestamp}

export SWCH_USE_HW_COLL=0

rm -f $output_file
bsub -J ${name} -q q_jiaweile -n $proc -share_size 15536  -b -cgsp 64 -o $output_file ../allreduce.bin