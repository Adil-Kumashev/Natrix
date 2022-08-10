#include "PsLegalMoveMaskGen.hpp"


Bitboard PsLegalMoveMaskGen::generate_pawn_default_mask(Pieces pieces, uint8_t side) {
    if (side == Pieces::White) {
        return (pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 8) & pieces._empty;
    }
    return (pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 8) & pieces._empty;
}
Bitboard PsLegalMoveMaskGen::generate_pawn_long_mask(Pieces pieces, uint8_t side) {
    Bitboard default_mask = PsLegalMoveMaskGen::generate_pawn_default_mask(pieces, side);

    if (side == Pieces::White) {
        return ((default_mask & BitboardRows::Rows[2]) << 8) & pieces._empty;
    }
    return ((default_mask & BitboardRows::Rows[5]) >> 8) & pieces._empty;
}
Bitboard PsLegalMoveMaskGen::generate_pawn_left_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures) {
    if (side == Pieces::White) {
        Bitboard mask = (pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 7) & BitboardColumns::InversionColumns[7];
        if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::Black];

        return mask;
    }

    Bitboard mask = (pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 9) & BitboardColumns::InversionColumns[7];
    if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::White];
    return mask;
}
Bitboard PsLegalMoveMaskGen::generate_pawn_right_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures) {
    if (side == Pieces::White) {
        Bitboard mask = (pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 9) & BitboardColumns::InversionColumns[0];
        if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::Black];

        return mask;
    }

    Bitboard mask = (pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 7) & BitboardColumns::InversionColumns[0];
    if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::White];
    return mask;
}
Bitboard PsLegalMoveMaskGen::generate_knight_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
    if (only_captures) {
        return KnightMasks::Masks[p] & pieces._side_bitboards[Pieces::inverse(side)];
    }
    return KnightMasks::Masks[p] & pieces._inversion_side_bitboards[side];
}
Bitboard PsLegalMoveMaskGen::generate_bishop_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
    Bitboard nw = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::NorthWest, false);
    Bitboard ne = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::NorthEast, false);
    Bitboard sw = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::SouthWest, true);
    Bitboard se = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::SouthEast, true);

    return nw | ne | sw | se;
}
Bitboard PsLegalMoveMaskGen::generate_rook_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
    Bitboard n = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::North, false);
    Bitboard s = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::South, true);
    Bitboard w = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::West, true);
    Bitboard e = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SlidersMasks::Direction::East, false);

    return n | s | w | e;
}
Bitboard PsLegalMoveMaskGen::generate_queen_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
    Bitboard bishop_mask = PsLegalMoveMaskGen::generate_bishop_mask(pieces, p, side, only_captures);
    Bitboard rook_mask = PsLegalMoveMaskGen::generate_rook_mask(pieces, p, side, only_captures);

    return bishop_mask | rook_mask;
}
Bitboard PsLegalMoveMaskGen::generate_king_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
    if (only_captures) {
        return KingMasks::Masks[p] & pieces._side_bitboards[Pieces::inverse(side)];
    }
    return KingMasks::Masks[p] & pieces._inversion_side_bitboards[side];
}
bool PsLegalMoveMaskGen::in_danger(Pieces pieces, uint8_t p, uint8_t side) {
    Bitboard opposite_pawns_left_captures = PsLegalMoveMaskGen::generate_pawn_left_captures_mask(pieces, Pieces::inverse(side), true);
    Bitboard opposite_pawns_right_captures = PsLegalMoveMaskGen::generate_pawn_right_captures_mask(pieces, Pieces::inverse(side), true);
    Bitboard opposite_pawns_captures = opposite_pawns_left_captures | opposite_pawns_right_captures;

    if (BitboardOperations::get_bit(opposite_pawns_captures, p)) return true;

    if (PsLegalMoveMaskGen::generate_knight_mask(pieces, p, side, true) & pieces._piece_bitboards[Pieces::inverse(side)][Pieces::Knight]) return true;
    if (PsLegalMoveMaskGen::generate_bishop_mask(pieces, p, side, true) & pieces._piece_bitboards[Pieces::inverse(side)][Pieces::Bishop]) return true;
    if (PsLegalMoveMaskGen::generate_rook_mask(pieces, p, side, true) & pieces._piece_bitboards[Pieces::inverse(side)][Pieces::Rook]) return true;
    if (PsLegalMoveMaskGen::generate_queen_mask(pieces, p, side, true) & pieces._piece_bitboards[Pieces::inverse(side)][Pieces::Queen]) return true;
    if (PsLegalMoveMaskGen::generate_king_mask(pieces, p, side, true) & pieces._piece_bitboards[Pieces::inverse(side)][Pieces::King]) return true;

    return false;
}
Bitboard PsLegalMoveMaskGen::_calc_ray(Pieces pieces, uint8_t p, uint8_t side, bool only_captures, uint8_t direction, bool bsr) {
    Bitboard blockers = SlidersMasks::Masks[p][direction] & pieces._all;

    if (blockers == 0) {
        if (only_captures) return 0;
        return SlidersMasks::Masks[p][direction];
    }

    uint8_t blocking_square;

    if (bsr) blocking_square = BitboardOperations::bsr(blockers);
    else blocking_square = BitboardOperations::bsf(blockers);

    Bitboard moves;

    if (only_captures) moves = 0;
    else moves = SlidersMasks::Masks[p][direction] ^ SlidersMasks::Masks[blocking_square][direction];

    if (BitboardOperations::get_bit(pieces._side_bitboards[side], blocking_square)) BitboardOperations::set_0(moves, blocking_square);
    else BitboardOperations::set_1(moves, blocking_square);

    return moves;
}