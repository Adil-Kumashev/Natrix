#include <iostream>
#include <cstdint>
#include <bit>
#include <array>
#include "../../../ANSI.hpp"


#pragma once


typedef uint64_t Bitboard;


namespace BitboardOperations {
    static constexpr void set_1(Bitboard &bb, uint8_t square) {
        bb = bb | (1ull << square);
    }
    static constexpr void set_0(Bitboard &bb, uint8_t square) {
        bb = bb & (~(1ull << square));
    }


    static constexpr bool get_bit(Bitboard bb, uint8_t square) {
        return (bb & (1ull << square));
    }


    static void print(Bitboard bb) {
        std::cout << ANSI::Green;

        for (int8_t y = 7; y >= 0; y = y - 1) {
            for (uint8_t x = 0; x < 8; x = x + 1) {
                std::cout << "|  ";

                if (BitboardOperations::get_bit(bb, y * 8 + x)) std::cout << "1";
                else std::cout << "0";

                std::cout << "  ";
            }
            std::cout << "|\n";
        }

        std::cout << ANSI::End;
    }


    static constexpr uint8_t count_1(Bitboard bb) {
        return std::popcount(bb);
    }


    static constexpr std::array<uint8_t, 64> BitScanTable = {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
    };


    static constexpr uint8_t bsf(Bitboard bb) {
        /* Не используется специальные инструкции процессора для переносимости скомпилированного кода.
         * Тесты показали, что BSFR инструкции у Intel не дают ощутимой производительности по сравнению с оптимизированным подходом.
         * Ниже оставлен код, позволяющий использовать BSFR инструкции (при наличии).*/

        // return __builtin_ctzll(bb);

        return BitboardOperations::BitScanTable[((bb ^ (bb - 1)) * 0x03f79d71b4cb0a89) >> 58];
    }
    static constexpr uint8_t bsr(Bitboard bb) {
        // См. комментарий к функции выше.

        // return 63 - __builtin_clzll(bb);

        bb = bb | (bb >> 1);
        bb = bb | (bb >> 2);
        bb = bb | (bb >> 4);
        bb = bb | (bb >> 8);
        bb = bb | (bb >> 16);
        bb = bb | (bb >> 32);

        return BitboardOperations::BitScanTable[(bb * 0x03f79d71b4cb0a89) >> 58];
    }
}


namespace BitboardRows {
    static consteval std::array<Bitboard, 8> calc_rows() {
        std::array<Bitboard, 8> rows{};

        for (uint8_t y = 0; y < 8; y = y + 1) {
            for (uint8_t x = 0; x < 8; x = x + 1) BitboardOperations::set_1(rows[y], y * 8 + x);
        }

        return rows;
    }


    static constexpr std::array<Bitboard, 8> Rows = BitboardRows::calc_rows();


    static consteval std::array<Bitboard, 8> calc_inversion_rows() {
        std::array<Bitboard, 8> inversion_rows{};

        for (uint8_t i = 0; i < 8; i = i + 1) inversion_rows[i] = ~Rows[i];

        return inversion_rows;
    }


    static constexpr std::array<Bitboard, 8> InversionRows = BitboardRows::calc_inversion_rows();
}


namespace BitboardColumns {
    static consteval std::array<Bitboard, 8> calc_columns() {
        std::array<Bitboard, 8> columns{};

        for (uint8_t x = 0; x < 8; x = x + 1) {
            for (uint8_t y = 0; y < 8; y = y + 1) BitboardOperations::set_1(columns[x], y * 8 + x);
        }

        return columns;
    }


    static constexpr std::array<Bitboard, 8> Columns = BitboardColumns::calc_columns();


    static consteval std::array<Bitboard, 8> calc_inversion_columns() {
        std::array<Bitboard, 8> inversion_columns{};

        for (uint8_t i = 0; i < 8; i = i + 1) inversion_columns[i] = ~Columns[i];

        return inversion_columns;
    }


    static constexpr std::array<Bitboard, 8> InversionColumns = BitboardColumns::calc_inversion_columns();
}