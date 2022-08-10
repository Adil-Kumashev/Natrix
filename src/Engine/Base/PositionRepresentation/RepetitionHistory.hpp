#include <vector>
#include "ZobristHash.hpp"


#pragma once


class RepetitionHistory {
public:
    RepetitionHistory();

    void add_position(ZobristHash hash);

    void clear();

    uint8_t get_repetition_number(ZobristHash hash);
private:
    std::vector<ZobristHash> _hashes;
};