#include "../PositionRepresentation/Position.hpp"
#include "KnightMasks.hpp"
#include "KingMasks.hpp"
#include "SlidersMasks.hpp"


#pragma once


class PsLegalMoveMaskGen {
public:
    static Bitboard generate_pawn_default_mask(Pieces pieces, uint8_t side);
    static Bitboard generate_pawn_long_mask(Pieces pieces, uint8_t side);
    static Bitboard generate_pawn_left_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures);
    static Bitboard generate_pawn_right_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures);

    static Bitboard generate_knight_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
    static Bitboard generate_bishop_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
    static Bitboard generate_rook_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
    static Bitboard generate_queen_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
    static Bitboard generate_king_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);

    static bool in_danger(Pieces pieces, uint8_t p, uint8_t side);
private:
    static Bitboard _calc_ray(Pieces pieces, uint8_t p, uint8_t side, bool only_captures, uint8_t direction, bool bsr);
};