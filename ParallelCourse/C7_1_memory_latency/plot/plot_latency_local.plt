set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/latency_local.png"

set xlabel "size of region (bytes)"
set ylabel "latency per load (CPU cycles)"
set title "latency per load in a random list traveral"

set xrange [4096:2147483648]
set logscale x 10
set xtics 10
set format x "10^{%L}"
set key left

plot './data/latency_local.dat' lw 1 with linespoints title "local"
