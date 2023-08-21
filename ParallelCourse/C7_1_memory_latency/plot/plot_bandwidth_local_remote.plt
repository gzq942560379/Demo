set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/bandwidth_local_remote.png"

set xlabel "size of region (bytes)"
set ylabel "bandwidth (GB/sec)"
set title "bandwidth of random list traveral"

set xrange [4096:2147483648]
set logscale x 10
set xtics 10
set format x "10^{%L}"
set key right

plot \
'./data/latency_local.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "local", \
'./data/latency_remote.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "remote",

