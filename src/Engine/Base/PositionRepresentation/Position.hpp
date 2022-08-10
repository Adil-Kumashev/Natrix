#include <cmath>
#include "RepetitionHistory.hpp"
#include "Move.hpp"


#pragma once


class Position {
public:
    Position();
    Position(const std::string& short_fen, uint8_t en_passant, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling, float move_ctr);

    friend std::ostream& operator <<(std::ostream& ostream, Position position);

    void move(Move move);

    Pieces _pieces;
    uint8_t _en_passant;

    bool _w_l_castling;
    bool _w_s_castling;
    bool _b_l_castling;
    bool _b_s_castling;

    bool _white_castling_happened;
    bool _black_castling_happened;

    float _move_ctr;
    ZobristHash _hash;
    float _fifty_moves_ctr;
    RepetitionHistory _repetition_history;
private:
    void _add_piece(uint8_t square, uint8_t type, uint8_t side);
    void _remove_piece(uint8_t square, uint8_t type, uint8_t side);
    void _change_en_passant(uint8_t en_passant);

    void _remove_w_l_castling();
    void _remove_w_s_castling();
    void _remove_b_l_castling();
    void _remove_b_s_castling();

    void _update_move_ctr();
    void _update_fifty_moves_ctr(bool break_event);
};