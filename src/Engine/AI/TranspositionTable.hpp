#include <set>
#include "Entry.hpp"


#pragma one


class TranspositionTable {
public:
    TranspositionTable();

    void add_entry(Entry entry);
    uint8_t try_to_find_best_move_index(ZobristHash hash);
private:
    std::set<Entry> _set;
};