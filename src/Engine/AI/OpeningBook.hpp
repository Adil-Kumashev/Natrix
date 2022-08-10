#include <fstream>
#include <sstream>
#include <tuple>
#include "../Base/MoveGeneration/LegalMoveGen.hpp"


#pragma once


class OpeningBook {
public:
    OpeningBook();
    OpeningBook(const std::string& path);

    std::tuple<Move, int32_t> try_to_find_move(const Position& position);
private:
    std::vector<std::vector<Move>> _moves;
};