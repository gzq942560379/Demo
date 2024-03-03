#!/bin/bash

output_file=main.out
rm -f $output_file
# bsub -J dgemm_test -q q_sw_expr -n 1 -share_size 15536  -b -cgsp 64 -ldm_share_mode 5 -ldm_share_size 64 -o $output_file ../dgemm.bin
bsub -I -J dgemm_test -q q_sw_expr -n 1 -share_size 15536  -b -cgsp 64 -o $output_file ./main.bin