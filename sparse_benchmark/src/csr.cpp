#include "csr.hpp"

template<typename IndexType, typename ValueType>
CSR<IndexType, ValueType>::CSR(const COO<IndexType,ValueType>& coo){
    if(!coo.is_sorted()){
        cerr << "In CSR<IndexType, ValueType>::CSR(const COO<IndexType,ValueType>& coo), coo have to be sorted !!!" << endl;
        throw std::invalid_argument("coo is not sorted");
    }

    row_ = coo.row_;
    col_ = coo.col_;
    nnz_ = coo.nnz_;
    sorted_ = true;

    // row_count
    // compute row_count
    vector<IndexType> row_count(row_, 0);
    for(IndexType i = 0; i < nnz_; ++i){
        row_count[coo.rowidx_[i]] += 1;
    }

    // rowptr nnz
    rowptr_.resize(row_ + 1);
    rowptr_[0] = 0;
    for(IndexType r = 0; r < row_; ++r){
        rowptr_[r + 1] = rowptr_[r] + row_count[r];
    }

    std::vector<IndexType> cur_index(row_);
    for(IndexType r = 0; r < row_; ++r){
        cur_index[r] = rowptr_[r];
    }

    // fill colidx value
    colidx_.resize(nnz_);
    value_.resize(nnz_);

    for(IndexType i = 0; i < nnz_; ++i){
        IndexType r = coo.rowidx_[i];
        IndexType csr_index = cur_index[r];
        colidx_[csr_index] = coo.colidx_[i];
        value_[csr_index] = coo.value_[i];
        cur_index[r] += 1;
    }

    // check
    for(IndexType r = 0; r < row_; ++r){
        assert(cur_index[r] == rowptr_[r+1]);
    }
}

template class CSR<int64_t, double>;

