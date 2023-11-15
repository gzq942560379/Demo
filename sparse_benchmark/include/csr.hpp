#pragma once

#include "common.hpp"
#include "coo.hpp"

template <typename IndexType, typename ValueType>
class CSR{
private:
    bool sorted_;
    
public:
    IndexType row_;
    IndexType col_;
    IndexType nnz_;
    vector<IndexType> rowptr_;
    vector<IndexType> colidx_;
    vector<ValueType> value_;

    CSR():row_(0),col_(0),nnz_(0),sorted_(false){}

    CSR(const COO<IndexType,ValueType>& coo);

    ~CSR(){}

    CSR(const CSR& other):row_(other.row_), col_(other.col_), nnz_(other.nnz_), rowptr_(other.rowptr_), colidx_(other.colidx_), value_(other.value_), sorted_(other.sorted_){
        cout << "Warning : CSR copy constructor is called !!!" << endl;
    }

    void spmv_benchmark();

    void SpMV(vector<ValueType>& y, const vector<ValueType>& x);

};
