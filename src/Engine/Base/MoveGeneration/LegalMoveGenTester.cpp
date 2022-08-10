#include "LegalMoveGenTester.hpp"


#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()


void LegalMoveGenTester::test() {
    Position position = {(std::string)LegalMoveGenTester::Fen, LegalMoveGenTester::EnPassant, LegalMoveGenTester::WLCastling, LegalMoveGenTester::WSCastling, LegalMoveGenTester::BLCastling, LegalMoveGenTester::BSCastling, 1};

    uint64_t correct;
    uint64_t got;

    uint64_t time_start;
    float speed;

    for (uint32_t i = 0; i < 6; i = i + 1) {
        time_start = nsecs;

        correct = LegalMoveGenTester::Nodes[i];
        got = LegalMoveGenTester::_get_nodes_number(position, Pieces::White, i);

        speed = (float)got / ((float)(nsecs - time_start) / (float)1e+9) / (float)1e+6;

        if (correct == got) std::cout << ANSI::Green << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. OK." << ANSI::End << std::endl;
        else std::cout << ANSI::Red << "Depth " << std::setw(4) << i << ". Correct: " << std::setw(18) << correct << ". Got: " << std::setw(18) << got << ". Speed: " << std::setw(10) << speed << " MNPS. Error." << ANSI::End << std::endl;
    }
}
uint64_t LegalMoveGenTester::_get_nodes_number(const Position& position, uint8_t side, uint32_t depth) {
    if (depth == 0) return 1;

    uint64_t ctr = 0;

    Position copy = position;

    MoveList moves = LegalMoveGen::generate(copy, side);
    Move move;

    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        move = moves[i];

        copy = position;
        copy.move(move);
        ctr = ctr + LegalMoveGenTester::_get_nodes_number(copy, Pieces::inverse(side), depth - 1);
    }

    return ctr;
}