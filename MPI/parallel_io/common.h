#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <mpi.h>
#include <omp.h>
#include <math.h>

// #define file_size static_cast<uint64_t>(1 << 10)  // 1KB
#define file_size (static_cast<uint64_t>(1) << 32)  // 2GB
#define element_type uint64_t
#define element_count (file_size / sizeof(element_type))

#define mpi_read_upper_bound_bytes (static_cast<int>(1) << 30)  // 2GB
#define mpi_read_upper_bound_elements (mpi_read_upper_bound_bytes / sizeof(element_type))  // 2GB

const std::string write_filepath = "data.dat";
const std::string read_filepath = "../data.dat";


