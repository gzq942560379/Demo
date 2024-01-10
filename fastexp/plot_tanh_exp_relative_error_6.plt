set term pngcairo font "Times-New-Roman,20" size 800,600
set output "./tanh_exp_relative_error_6.png"

set xlabel "x"
set ylabel "diff"
set title "tanh exp diff"

# set xrange [-1:1]

plot \
'./tanh_exp_6.dat' using 1:5 lw 1 with linespoints title "tanh_exp_6", \
