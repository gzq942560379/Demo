#pragma once

#include <vector>
#include <string>
#include <typeinfo>
#include <complex>
#include <iostream>
#include <cstdint>
#include <cstdbool>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#ifndef _OPENMP
#error "must open -fopenmp flag!!!"
#endif
#include <omp.h>
#include <omp_timer.hpp>
#include <env.hpp>

using std::vector;
using std::string;
using std::complex;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::tuple;
using std::make_tuple;
using std::get;


