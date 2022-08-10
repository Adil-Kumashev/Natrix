#include <cstdint>
#include <array>


#pragma once


namespace ZobristHashConsteval {
    namespace PRNG {
        static constexpr uint64_t Seed = 0x98f107;
        static constexpr uint64_t Multiplier = 0x71abc9;
        static constexpr uint64_t Summand = 0xff1b3f;
    }


    static consteval uint64_t next_random(uint64_t previous) {
        return ZobristHashConsteval::PRNG::Multiplier * previous + ZobristHashConsteval::PRNG::Summand;
    }
    static consteval std::array<std::array<std::array<uint64_t, 6>, 2>, 64> calc_constants() {
        std::array<std::array<std::array<uint64_t, 6>, 2>, 64> constants{};

        uint64_t previous = ZobristHashConsteval::PRNG::Seed;

        for (uint8_t square = 0; square < 64; square = square + 1) {
            for (uint8_t side = 0; side < 2; side = side + 1) {
                for (uint8_t type = 0; type < 6; type = type + 1) {
                    previous = ZobristHashConsteval::next_random(previous);
                    constants[square][side][type] = previous;
                }
            }
        }

        return constants;
    }


    static constexpr std::array<std::array<std::array<uint64_t, 6>, 2>, 64> Constants = calc_constants();
    static constexpr uint64_t BlackMove = ZobristHashConsteval::next_random(ZobristHashConsteval::Constants[63][1][5]);
    static constexpr uint64_t EnPassant = ZobristHashConsteval::next_random(ZobristHashConsteval::BlackMove);
    static constexpr uint64_t WhiteLongCastling = ZobristHashConsteval::next_random(ZobristHashConsteval::EnPassant);
    static constexpr uint64_t WhiteShortCastling = ZobristHashConsteval::next_random(ZobristHashConsteval::WhiteLongCastling);
    static constexpr uint64_t BlackLongCastling = ZobristHashConsteval::next_random(ZobristHashConsteval::WhiteShortCastling);
    static constexpr uint64_t BlackShortCastling = ZobristHashConsteval::next_random(ZobristHashConsteval::BlackLongCastling);
}