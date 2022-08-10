#include <ostream>
#include <cctype>
#include "Bitboard.hpp"


#pragma once


struct Pieces {
    Pieces();
    Pieces(const std::string& short_fen);

    friend std::ostream& operator <<(std::ostream& ostream, Pieces pieces);
    friend bool operator ==(Pieces left, Pieces right);

    void update_bitboards();

    static uint8_t inverse(uint8_t side);

    std::array<std::array<Bitboard, 6>, 2> _piece_bitboards{};
    std::array<Bitboard, 2> _side_bitboards{};
    std::array<Bitboard, 2> _inversion_side_bitboards{};
    Bitboard _all;
    Bitboard _empty;

    static constexpr uint8_t Pawn = 0;
    static constexpr uint8_t Knight = 1;
    static constexpr uint8_t Bishop = 2;
    static constexpr uint8_t Rook = 3;
    static constexpr uint8_t Queen = 4;
    static constexpr uint8_t King = 5;

    static constexpr uint8_t White = 0;
    static constexpr uint8_t Black = 1;
};