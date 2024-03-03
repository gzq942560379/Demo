#!/bin/bash


output_file=main.out
rm -f $output_file
bsub -I -J main -q q_sw_expr -n 1 -share_size 15536  -b -cgsp 64 -cache_size 0 -o $output_file ./main.bin