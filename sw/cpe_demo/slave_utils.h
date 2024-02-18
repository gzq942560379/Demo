#pragma once

#include <crts.h>
#include <stdint.h>

// #if defined(WM_SP)
//     #define MPI_SCALAR MPI_FLOAT
//     typedef float scalar;
// #elif defined(WM_DP)
//     #define MPI_SCALAR MPI_DOUBLE
//     typedef double scalar;
// #elif defined(WM_LP)
//     #define MPI_SCALAR MPI_LONG_DOUBLE
//     typedef long double scalar;
// #endif

// #if WM_ARCH_OPTION == 32
//     #define MPI_LABEL MPI_INT
//     typedef int32_t label;
// #elif WM_ARCH_OPTION == 64
//     #define MPI_LABEL MPI_LONG
//     typedef int64_t label;
// #else
//     #error "WM_ARCH_OPTION must be 32 or 64"
// #endif

#define slave_min(x,y) ((x)<(y)?(x):(y))
#define slave_max(x,y) ((x)<(y)?(y):(x))

#define LOCAL_START(n,rank,size) ((rank) * (n) / (size))
#define LOCAL_END(n,rank,size) (((rank) + 1) * (n) / (size))

#define slave_div_ceil(a,b) (((a) + (b) - 1) / (b))