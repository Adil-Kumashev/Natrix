#include "ZobristHash.hpp"


ZobristHash::ZobristHash() = default;
ZobristHash::ZobristHash(Pieces pieces, bool black_move, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling) {
    this->_hash = 0;

    if (black_move) this->invert_move();
    if (w_l_castling) this->invert_w_l_castling();
    if (w_s_castling) this->invert_w_s_castling();
    if (b_l_castling) this->invert_b_l_castling();
    if (b_s_castling) this->invert_b_s_castling();

    uint8_t side;
    for (uint8_t square = 0; square < 64; square = square + 1) {
        if (BitboardOperations::get_bit(pieces._side_bitboards[Pieces::White], square)) side = Pieces::White;
        else if (BitboardOperations::get_bit(pieces._side_bitboards[Pieces::Black], square)) side = Pieces::Black;
        else continue;

        for (uint8_t type = 0; type < 6; type = type + 1) {
            if (BitboardOperations::get_bit(pieces._piece_bitboards[side][type], square)) {
                this->invert_piece(square, type, side);
                break;
            }
        }
    }
}
bool operator ==(ZobristHash left, ZobristHash right) {
    return (left._hash == right._hash);
}
bool operator <(ZobristHash left, ZobristHash right) {
    return (left._hash < right._hash);
}
void ZobristHash::invert_piece(uint8_t square, uint8_t type, uint8_t side) {
    this->_hash = this->_hash ^ ZobristHashConsteval::Constants[square][side][type];
}
void ZobristHash::invert_move() {
    this->_hash = this->_hash ^ ZobristHashConsteval::BlackMove;
}
void ZobristHash::invert_w_l_castling() {
    this->_hash = this->_hash ^ ZobristHashConsteval::WhiteLongCastling;
}
void ZobristHash::invert_w_s_castling() {
    this->_hash = this->_hash ^ ZobristHashConsteval::WhiteShortCastling;
}
void ZobristHash::invert_b_l_castling() {
    this->_hash = this->_hash ^ ZobristHashConsteval::BlackLongCastling;
}
void ZobristHash::invert_b_s_castling() {
    this->_hash = this->_hash ^ ZobristHashConsteval::BlackShortCastling;
}