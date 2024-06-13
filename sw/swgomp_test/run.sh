#!/bin/bash

output_file=vfma.out
rm -f $output_file
bsub -I -J vfma -q q_sw_expr -n 1 -share_size 15536  -b -cgsp 64 -o $output_file ./vfma.bin