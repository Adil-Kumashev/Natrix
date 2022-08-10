#include "../Base/MoveGeneration/MoveList.hpp"
#include "StaticEvaluator.hpp"


#pragma once


class MoveSorter {
public:
    static MoveList sort(Pieces pieces, MoveList moves);
private:
    static int32_t _evaluate_move(Pieces pieces, Move move);
};