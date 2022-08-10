#include "../PositionRepresentation/Bitboard.hpp"


#pragma once


namespace SlidersMasks {
    struct Direction {
        static constexpr int8_t North = 0;
        static constexpr int8_t South = 1;
        static constexpr int8_t West = 2;
        static constexpr int8_t East = 3;

        static constexpr int8_t NorthWest = 4;
        static constexpr int8_t NorthEast = 5;
        static constexpr int8_t SouthWest = 6;
        static constexpr int8_t SouthEast = 7;
    };


    static consteval Bitboard calc_mask(uint8_t p, int8_t direction) {
        Bitboard mask = 0;

        int8_t x = p % 8;
        int8_t y = p / 8;

        for (; ;) {
            switch (direction) {
                case SlidersMasks::Direction::North: y = y + 1; break;
                case SlidersMasks::Direction::South: y = y - 1; break;
                case SlidersMasks::Direction::West: x = x - 1; break;
                case SlidersMasks::Direction::East: x = x + 1; break;

                case SlidersMasks::Direction::NorthWest: y = y + 1; x = x - 1; break;
                case SlidersMasks::Direction::NorthEast: y = y + 1; x = x + 1; break;
                case SlidersMasks::Direction::SouthWest: y = y - 1; x = x - 1; break;
                case SlidersMasks::Direction::SouthEast: y = y - 1; x = x + 1; break;
            }

            if (x > 7 or x < 0 or y > 7 or y < 0) break;

            BitboardOperations::set_1(mask, y * 8 + x);
        }

        return mask;
    }


    static consteval std::array<std::array<Bitboard, 8>, 64> calc_masks() {
        std::array<std::array<Bitboard, 8>, 64> masks{};

        for (uint8_t i = 0; i < 64; i = i + 1) {
            for (uint8_t j = 0; j < 8; j = j + 1) masks[i][j] = SlidersMasks::calc_mask(i, j);
        }

        return masks;
    }


    static constexpr std::array<std::array<Bitboard, 8>, 64> Masks = SlidersMasks::calc_masks();
};