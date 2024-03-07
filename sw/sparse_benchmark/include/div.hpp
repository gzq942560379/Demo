#pragma once

#include "common.hpp"
#include "coo.hpp"

#define ROW_BLOCK_BIT 5

#define DIV_BLOCK_MASK ((1 << (row_block_bit_)) - 1)
#define DIV_BLOCK_MASK_INVERSE (~DIV_BLOCK_MASK)

#define DIV_BLOCK_INDEX(row) ((row) >> (row_block_bit_))
#define DIV_BLOCK_ROW(row) ((row) & DIV_BLOCK_MASK)
#define DIV_BLOCK_START(bi) (bi << row_block_bit_)
// #define DIV_BLOCK_END(rbs) (std::min((rbs) + (row_block_size_), (row_)))
#define DIV_BLOCK_END(rbs) ((rbs) + (row_block_size_))
// #define DIV_BLOCK_LEN(rbs,rbe) ((rbe - rbs))
#define DIV_BLOCK_LEN(rbs,rbe) (row_block_size_)
#define DIV_BLOCK_TAIL DIV_BLOCK_ROW(row_)
#define DIV_BLOCK_COUNT (((row_) + (row_block_size_) - 1) >> (row_block_bit_))

#define DIV_INDEX_BLOCK_START(rbs) ((rbs) * (distance_count_))
#define DIV_COL_OFFSET(divcol) ((divcol) << (row_block_bit_))
#define DIV_INDEX(row,divcol) (((row) & (DIV_BLOCK_MASK_INVERSE)) * (distance_count_) + DIV_COL_OFFSET(divcol) + DIV_BLOCK_ROW(row))


template <typename IndexType, typename ValueType>
class DIV{
private:


public:
    const IndexType row_block_bit_; // row_block_size_ = 2^row_block_bit_
    const IndexType row_block_size_;

    IndexType row_;
    vector<ValueType> diag_value_;

    IndexType block_count_; // node count 只算能整除的部分
    IndexType block_tail_; // 

    IndexType distance_count_;
    vector<IndexType> distance_list_;
    IndexType max_distance_;
    IndexType min_distance_;

    IndexType head_block_count_;
    IndexType tail_block_count_;

    vector<scalar> off_diag_value_;

    DIV():row_(0),row_block_bit_(ROW_BLOCK_BIT),row_block_size_(1 << row_block_bit_);
    
    DIV(const COO<IndexType,ValueType>& coo);



}