#!/bin/bash 

# ./run.sh

source /vol0004/apps/oss/spack/share/spack/setup-env.sh

spack load gnuplot@5.4.3%gcc@12.2.0 arch=linux-rhel8-cascadelake

gnuplot ../plot_fastexp_relative_error_1.plt
gnuplot ../plot_fastexp_relative_error_2.plt
gnuplot ../plot_fastexp_relative_error_3.plt
gnuplot ../plot_fastexp_relative_error_4.plt
gnuplot ../plot_fastexp_relative_error_5.plt
gnuplot ../plot_fastexp_relative_error_6.plt
gnuplot ../plot_fastexp_relative_error_7.plt
gnuplot ../plot_fastexp_relative_error_8.plt

gnuplot ../plot_tanh_exp_relative_error_1.plt
gnuplot ../plot_tanh_exp_relative_error_2.plt
gnuplot ../plot_tanh_exp_relative_error_3.plt
gnuplot ../plot_tanh_exp_relative_error_4.plt
gnuplot ../plot_tanh_exp_relative_error_5.plt
gnuplot ../plot_tanh_exp_relative_error_6.plt
gnuplot ../plot_tanh_exp_relative_error_7.plt
gnuplot ../plot_tanh_exp_relative_error_8.plt
