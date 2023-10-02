set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./png/performance_dmvm_unroll.png"

set xlabel "R"
set ylabel "MF/s"

set xrange [256:524288]
set logscale x 10
set xtics 1e3, 10
set format x "10^{%L}"
set key right

plot \
'./data/dmvm_mflops_naive.dat' with linespoints lw 1 title "naive", \
'./data/dmvm_mflops_unroll2.dat' with linespoints lw 1 title "unroll2", \

# plot \
'./data/dmvm_mflops_avx512.dat' with linespoints lw 1 title "avx512", \
'./data/dmvm_mflops_avx512_unroll2.dat' with linespoints lw 1 title "avx512 unroll2", \
'./data/dmvm_mflops_avx512_unroll4.dat' with linespoints lw 1 title "avx512 unroll4", \


