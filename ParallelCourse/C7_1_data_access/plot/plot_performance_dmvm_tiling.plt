set term pngcairo font "Times-New-Roman,20" size 600,800
set output "./png/performance_dmvm_tiling.png"

set xlabel "number of rows R"
set ylabel "MF/s"

# set yrange [0:]
set xrange [256:524288]
set logscale x 10
set xtics 1e3, 10
set format x "10^{%L}"
set key bottom right

plot \
'./data/dmvm_mflops_naive.dat' with linespoints lw 1 title "Plain", \
'./data/dmvm_mflops_tiling2048.dat' with linespoints lw 1 title "R_b=2048", \
'./data/dmvm_mflops_tiling8192.dat' with linespoints lw 1 title "R_b=8192", \



# './data/dmvm_mflops_tiling1024.dat' with linespoints lw 1 title "R_b=1024", \
# './data/dmvm_mflops_tiling4096.dat' with linespoints lw 1 title "R_b=4096", \
# 
