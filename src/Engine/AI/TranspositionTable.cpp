#include "TranspositionTable.hpp"


TranspositionTable::TranspositionTable() = default;
void TranspositionTable::add_entry(Entry entry) {
    auto hash_copy = this->_set.find(entry);
    if (hash_copy == this->_set.end() or hash_copy->_depth < entry._depth) this->_set.insert(entry);
}
uint8_t TranspositionTable::try_to_find_best_move_index(ZobristHash hash) {
    auto entry = this->_set.find({hash, 0, 0});

    if (entry == this->_set.end()) return 255;
    return entry->_best_move_index;
}