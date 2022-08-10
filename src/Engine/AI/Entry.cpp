#include "Entry.hpp"


Entry::Entry() = default;
Entry::Entry(ZobristHash hash, int32_t depth, uint8_t best_move_index) {
    this->_hash = hash;
    this->_depth = depth;
    this->_best_move_index = best_move_index;
}
bool operator <(Entry left, Entry right) {
    return (left._hash < right._hash);
}