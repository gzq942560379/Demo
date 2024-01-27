#pragma once

#include "common.hpp"

template <typename IndexType, typename ValueType>
class COO{
private:
    bool sorted_;

public:
    IndexType row_;
    IndexType col_;
    IndexType nnz_;
    vector<IndexType> rowidx_;
    vector<IndexType> colidx_;
    vector<ValueType> value_;

    COO():row_(0),col_(0),nnz_(0),sorted_(false){}
    ~COO(){}

    COO(IndexType row, IndexType col, IndexType nnz, vector<IndexType> rowidx, vector<IndexType> colidx, vector<ValueType> value)
        :row_(row), col_(col), nnz_(nnz), rowidx_(rowidx), colidx_(colidx), value_(value), sorted_(false){}

    COO(const COO& other):row_(other.row_), col_(other.col_), nnz_(other.nnz_), rowidx_(other.rowidx_), colidx_(other.colidx_), value_(other.value_), sorted_(other.sorted_){
        cout << "Warning : COO copy constructor is called !!!" << endl;
    }

    bool is_sorted() const {return sorted_;};

    void sort();

    bool check_sorted();

    void print_info();

};
