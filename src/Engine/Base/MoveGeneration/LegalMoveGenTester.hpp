#include <chrono>
#include <iomanip>
#include "LegalMoveGen.hpp"


#pragma once


class LegalMoveGenTester {
public:
    static void test();
private:
    static uint64_t _get_nodes_number(const Position& position, uint8_t side, uint32_t depth);

    static constexpr std::string_view Fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R";
    static constexpr uint8_t EnPassant = 255;
    static constexpr bool WLCastling = true;
    static constexpr bool WSCastling = true;
    static constexpr bool BLCastling = false;
    static constexpr bool BSCastling = false;

    static constexpr std::array<uint64_t, 6> Nodes = {1, 44, 1486, 62379, 2103487, 89941194};
};