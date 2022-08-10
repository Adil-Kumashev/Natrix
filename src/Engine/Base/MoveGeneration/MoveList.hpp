#include <array>
#include "../PositionRepresentation/Move.hpp"
#include "../PositionRepresentation/Pieces.hpp"


#pragma once


class MoveList {
public:
    MoveList();

    Move &operator[](uint8_t index);
    Move operator[](uint8_t index) const;

    void push_back(Move move);
    [[nodiscard]] uint8_t size() const;
private:
    std::array<Move, 218> _moves{};
    uint8_t _size;
};