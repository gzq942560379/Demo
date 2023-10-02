set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/performance_dmvm_tiling.png"

set xlabel "R"
set ylabel "MF/s"

set xrange [256:524288]
set logscale x 10
set xtics 1e3, 10
set format x "10^{%L}"
set key right

plot \
'./data/dmvm_mflops_naive.dat' with linespoints lw 1 title "Plain", \
'./data/dmvm_mflops_tiling100.dat' with linespoints lw 1 title "1D Blocking: R_b=100", \
'./data/dmvm_mflops_tiling1000.dat' with linespoints lw 1 title "1D Blocking: R_b=1000", \
'./data/dmvm_mflops_tiling10000.dat' with linespoints lw 1 title "1D Blocking: R_b=10000", \


