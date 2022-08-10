#include "Pieces.hpp"
#include "ZobristHashConstants.hpp"


#pragma once


struct ZobristHash {
    ZobristHash();
    ZobristHash(Pieces pieces, bool black_move, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling);

    friend bool operator ==(ZobristHash left, ZobristHash right);
    friend bool operator <(ZobristHash left, ZobristHash right);

    void invert_piece(uint8_t square, uint8_t type, uint8_t side);
    void invert_move();
    void invert_w_l_castling();
    void invert_w_s_castling();
    void invert_b_l_castling();
    void invert_b_s_castling();

    uint64_t _hash;
};