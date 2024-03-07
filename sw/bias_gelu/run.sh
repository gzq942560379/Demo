#!/bin/bash


output_file=bias_gelu.out
rm -f $output_file
bsub -I -J bias_gelu -q q_sw_expr -n 1 -share_size 15536  -b -cgsp 64 -o $output_file ./bias_gelu.bin