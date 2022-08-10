#include <chrono>
#include <future>
#include <unistd.h>
#include <iomanip>
#include "../Base/MoveGeneration/LegalMoveGen.hpp"
#include "MoveSorter.hpp"
#include "TranspositionTable.hpp"
#include "OpeningBook.hpp"


#pragma once


class AI {
public:
    AI();
    AI(const std::string& opening_book_path);

    Move best_move(const Position& position, uint8_t side, int32_t min_ms, int32_t max_ms);
private:
    OpeningBook _opening_book;

    static std::tuple<int32_t, Move> _best_move(const Position& position, uint8_t side, int32_t depth, TranspositionTable &tt);

    static std::tuple<int32_t, Move> _alpha_beta_min(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable &tt);
    static std::tuple<int32_t, Move> _alpha_beta_max(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable &tt);

    static int32_t _alpha_beta_min_only_captures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current);
    static int32_t _alpha_beta_max_only_captures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current);

    struct Infinity {
        static constexpr int32_t Negative = -1e+9;
        static constexpr int32_t Positive = 1e+9;
    };
};