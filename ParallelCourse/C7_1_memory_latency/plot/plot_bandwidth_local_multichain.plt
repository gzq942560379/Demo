set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/bandwidth_local_multichain.png"

set xlabel "size of region (bytes)"
set ylabel "bandwidth (GB/sec)"
set title "bandwidth with a number of chains"

set xrange [4096:2147483648]
set logscale x 10
set xtics 10
set format x "10^{%L}"
set key right

plot \
'./data/latency_local.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "1 chains", \
'./data/latency_02.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "2 chains", \
'./data/latency_04.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "4 chains", \
'./data/latency_06.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "6 chains", \
'./data/latency_08.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "8 chains", \
'./data/latency_10.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "10 chains", \
'./data/latency_12.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "12 chains", \
'./data/latency_14.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "14 chains", \
'./data/latency_16.dat' using 1:(64/$2*2.9*1e9/1024/1024/1024) lw 1 with linespoints title "16 chains", \


