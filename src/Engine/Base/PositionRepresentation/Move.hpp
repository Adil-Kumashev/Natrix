#include <cstdint>


#pragma once


struct Move {
    Move();
    Move(uint8_t from, uint8_t to, uint8_t attacker_type, uint8_t attacker_side, uint8_t defender_type, uint8_t defender_side, uint8_t flag = Move::Flag::Default);

    friend bool operator ==(Move left, Move right);

    uint8_t _from;
    uint8_t _to;

    uint8_t _attacker_type;
    uint8_t _attacker_side;

    uint8_t _defender_type;
    uint8_t _defender_side;

    uint8_t _flag;

    struct Flag {
        static constexpr uint8_t Default = 0;

        static constexpr uint8_t PawnLongMove = 1;
        static constexpr uint8_t EnPassantCapture = 2;

        static constexpr uint8_t WhiteLongCastling = 3;
        static constexpr uint8_t WhiteShortCastling = 4;
        static constexpr uint8_t BlackLongCastling = 5;
        static constexpr uint8_t BlackShortCastling = 6;

        static constexpr uint8_t PromoteToKnight = 7;
        static constexpr uint8_t PromoteToBishop = 8;
        static constexpr uint8_t PromoteToRook = 9;
        static constexpr uint8_t PromoteToQueen = 10;
    };
};