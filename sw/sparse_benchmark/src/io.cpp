#include "mmio.h"
#include "io.hpp"

COO<int64_t, double> read_coo_from_pattern_mtx(string filename){

    int M,N,nz,*I,*J;

    mm_read_unsymmetric_sparse_pattern(filename.c_str(), &M, &N, &nz, &I, &J);
    
    int64_t row, col, nnz;
    row = M;
    col = N;
    nnz = nz;

    vector<int64_t> rowidx(nnz);
    vector<int64_t> colidx(nnz);
    vector<double> value(nnz);

    for(int64_t i = 0; i < nnz; ++i){
        rowidx[i] = I[i];
        colidx[i] = J[i];
        value[i] = 1.;
    }

    free(I);
    free(J);

    COO<int64_t, double> ret(row, col, nnz, rowidx, colidx, value);
    // if(!ret.check_sorted()){
        ret.sort();
    // }
    ret.check_sorted();
    ret.print_info();
    return ret;
}
