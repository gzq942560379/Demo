#include "coo.hpp"

template<typename IndexType, typename ValueType>
void COO<IndexType, ValueType>::sort(){
    vector<tuple<IndexType,IndexType,ValueType>> rcv_vector(nnz_);
    for(IndexType i = 0; i < nnz_; ++i){
        rcv_vector[i] = make_tuple(rowidx_[i], colidx_[i], value_[i]);
    }
    auto compare = [](const tuple<IndexType,IndexType,ValueType>& a, const tuple<IndexType,IndexType,ValueType>& b){
        if(get<0>(a) == get<0>(b)) 
            return get<1>(a) < get<1>(b);
        return get<0>(a) < get<0>(b);
    };
    std::sort(rcv_vector.begin(), rcv_vector.end(), compare);
    for(IndexType i = 0; i < nnz_; ++i){
        rowidx_[i] = get<0>(rcv_vector[i]);
        colidx_[i] = get<1>(rcv_vector[i]);
        value_[i] = get<2>(rcv_vector[i]);
    }
    sorted_ = true;
}

template<typename IndexType, typename ValueType>
void COO<IndexType, ValueType>::print_info(){
    cout << "row : " << row_ << endl;
    cout << "col : " << col_ << endl;
    cout << "nnz : " << nnz_ << endl;
    cout << "sorted : " << sorted_ << endl;
    // for(IndexType i = 0; i < nnz_; ++i){
    //     cout << rowidx_[i] << ", " << colidx_[i] << ", " << value_[i] << endl;
    // }
}

template<typename IndexType, typename ValueType>
bool COO<IndexType, ValueType>::check_sorted(){
    for(IndexType i = 1; i < nnz_; ++i){
        if(rowidx_[i-1] < rowidx_[i])
            continue;
        if(colidx_[i-1] < colidx_[i])
            continue; 
        sorted_ = false;
        return sorted_;
    }
    sorted_ = true;
    return sorted_;
}

template class COO<int64_t, double>;
