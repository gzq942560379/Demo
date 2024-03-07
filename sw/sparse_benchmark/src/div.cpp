#include "div.hpp"

template<typename IndexType, typename ValueType>
DIV<IndexType, ValueType>::DIV(const COO<IndexType,ValueType>& coo){
    if(!coo.is_sorted()){
        cerr << "In DIV<IndexType, ValueType>::DIV(const COO<IndexType,ValueType>& coo), coo have to be sorted !!!" << endl;
        throw std::invalid_argument("coo is not sorted");
    }

    row_ = coo.row_;
    
}

template class DIV<int64_t, double>;

