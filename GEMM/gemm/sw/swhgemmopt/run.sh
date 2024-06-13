# usage M N K, rowmajor
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 2048 20
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 4096 2048
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 2048 4096
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 1024 2048
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 512 2048
bsub -I -q q_sw_expr -n 1 -np 1 -cgsp 64 -share_size 15000 -cache_size 0 -b ./hgemm 32768 20 512
