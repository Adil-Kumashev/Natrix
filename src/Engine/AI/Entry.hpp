#include "../Base/PositionRepresentation/ZobristHash.hpp"


#pragma once


struct Entry {
    Entry();
    Entry(ZobristHash hash, int32_t depth, uint8_t best_move_index);

    friend bool operator <(Entry left, Entry right);

    ZobristHash _hash;
    int32_t _depth;
    uint8_t _best_move_index;
};