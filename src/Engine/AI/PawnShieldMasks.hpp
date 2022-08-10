#include <array>
#include "../Base/PositionRepresentation/Bitboard.hpp"


#pragma once


namespace PawnShieldMasks {
    static consteval std::array<Bitboard, 64> calc_white_pawn_shield_masks() {
        std::array<Bitboard, 64> white_pawn_shield_masks{};

        for (uint8_t x = 0; x < 8; x = x + 1) {
            for (uint8_t y = 0; y < 7; y = y + 1) {
                if (x != 0) BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 1) * 8 + x - 1);
                if (x != 7) BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 1) * 8 + x + 1);
                BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 1) * 8 + x);

                if (y != 6) {
                    if (x != 0) BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 2) * 8 + x - 1);
                    if (x != 7) BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 2) * 8 + x + 1);
                    BitboardOperations::set_1(white_pawn_shield_masks[y * 8 + x], (y + 2) * 8 + x);
                }
            }
        }

        return white_pawn_shield_masks;
    }


    static consteval std::array<Bitboard, 64> calc_black_pawn_shield_masks() {
        std::array<Bitboard, 64> black_pawn_shield_masks{};

        for (uint8_t x = 0; x < 8; x = x + 1) {
            for (uint8_t y = 1; y < 8; y = y + 1) {
                if (x != 0) BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 1) * 8 + x - 1);
                if (x != 7) BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 1) * 8 + x + 1);
                BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 1) * 8 + x);

                if (y != 1) {
                    if (x != 0) BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 2) * 8 + x - 1);
                    if (x != 7) BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 2) * 8 + x + 1);
                    BitboardOperations::set_1(black_pawn_shield_masks[y * 8 + x], (y - 2) * 8 + x);
                }
            }
        }

        return black_pawn_shield_masks;
    }


    static constexpr std::array<Bitboard, 64> WhitePawnShieldMasks = PawnShieldMasks::calc_white_pawn_shield_masks();
    static constexpr std::array<Bitboard, 64> BlackPawnShieldMasks = PawnShieldMasks::calc_black_pawn_shield_masks();
}