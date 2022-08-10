#include "Position.hpp"


Position::Position() = default;
Position::Position(const std::string& short_fen, uint8_t en_passant, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling, float move_ctr) {
    this->_pieces = {short_fen};
    this->_en_passant = en_passant;

    this->_w_l_castling = w_l_castling;
    this->_w_s_castling = w_s_castling;
    this->_b_l_castling = b_l_castling;
    this->_b_s_castling = b_s_castling;

    // Этими переменными пользуется ИИ при оценке позиции. На генерацию рокировок влияют только переменные выше.
    this->_white_castling_happened = false;
    this->_black_castling_happened = false;

    this->_move_ctr = move_ctr;
    this->_hash = {this->_pieces, (this->_move_ctr - std::floor(this->_move_ctr) > 1e-4), this->_w_l_castling, this->_w_s_castling, this->_b_l_castling, this->_b_s_castling};
    this->_repetition_history.add_position(this->_hash);
    this->_fifty_moves_ctr = 0;
}
std::ostream &operator<<(std::ostream &ostream, Position position) {
    ostream << position._pieces;

    ostream << ANSI::Green;

    ostream << "En passant: " << (uint32_t)position._en_passant << "\n";
    ostream << "White long castling: " << position._w_l_castling << "\n";
    ostream << "White short castling: " << position._w_s_castling << "\n";
    ostream << "Black long castling: " << position._b_l_castling << "\n";
    ostream << "Black short castling: " << position._b_l_castling << "\n";
    ostream << "Move counter: " << position._move_ctr << "\n";
    ostream << "Zobrist hash: " << std::hex << "0x" << position._hash._hash << "\n" << std::dec;
    ostream << "Fifty moves counter: " << position._fifty_moves_ctr << "\n";
    ostream << "Threefold repetition counter: " << (uint32_t)position._repetition_history.get_repetition_number(position._hash) << "\n";

    ostream << ANSI::End;

    return ostream;
}
void Position::move(Move move) {
    this->_remove_piece(move._from, move._attacker_type, move._attacker_side);
    this->_add_piece(move._to, move._attacker_type, move._attacker_side);
    if (move._defender_type != 255) this->_remove_piece(move._to, move._defender_type, move._defender_side);

    switch (move._flag) {
        case Move::Flag::Default:
            break;

        case Move::Flag::PawnLongMove:
            this->_change_en_passant((move._from + move._to) / 2);
            break;
        case Move::Flag::EnPassantCapture:
            if (move._attacker_side == Pieces::White) this->_remove_piece(move._to - 8, Pieces::Pawn, Pieces::Black);
            else this->_remove_piece(move._to + 8, Pieces::Pawn, Pieces::White);
            break;

        case Move::Flag::WhiteLongCastling:
            this->_remove_piece(0, Pieces::Rook, Pieces::White);
            this->_add_piece(3, Pieces::Rook, Pieces::White);
            this->_white_castling_happened = true;
            break;
        case Move::Flag::WhiteShortCastling:
            this->_remove_piece(7, Pieces::Rook, Pieces::White);
            this->_add_piece(5, Pieces::Rook, Pieces::White);
            this->_white_castling_happened = true;
            break;
        case Move::Flag::BlackLongCastling:
            this->_remove_piece(56, Pieces::Rook, Pieces::Black);
            this->_add_piece(59, Pieces::Rook, Pieces::Black);
            this->_black_castling_happened = true;
            break;
        case Move::Flag::BlackShortCastling:
            this->_remove_piece(63, Pieces::Rook, Pieces::Black);
            this->_add_piece(61, Pieces::Rook, Pieces::Black);
            this->_black_castling_happened = true;
            break;

        case Move::Flag::PromoteToKnight:
            this->_remove_piece(move._to, Pieces::Pawn, move._attacker_side);
            this->_add_piece(move._to, Pieces::Knight, move._attacker_side);
            break;
        case Move::Flag::PromoteToBishop:
            this->_remove_piece(move._to, Pieces::Pawn, move._attacker_side);
            this->_add_piece(move._to, Pieces::Bishop, move._attacker_side);
            break;
        case Move::Flag::PromoteToRook:
            this->_remove_piece(move._to, Pieces::Pawn, move._attacker_side);
            this->_add_piece(move._to, Pieces::Rook, move._attacker_side);
            break;
        case Move::Flag::PromoteToQueen:
            this->_remove_piece(move._to, Pieces::Pawn, move._attacker_side);
            this->_add_piece(move._to, Pieces::Queen, move._attacker_side);
            break;
    }

    this->_pieces.update_bitboards();

    if (move._flag != Move::Flag::PawnLongMove) this->_change_en_passant(255);

    switch (move._from) {
        case 0:
            this->_remove_w_l_castling();
            break;
        case 4:
            this->_remove_w_l_castling();
            this->_remove_w_s_castling();
            break;
        case 7:
            this->_remove_w_s_castling();
            break;
        case 56:
            this->_remove_b_l_castling();
            break;
        case 60:
            this->_remove_b_l_castling();
            this->_remove_b_s_castling();
            break;
        case 63:
            this->_remove_b_s_castling();
            break;
    }

    this->_update_move_ctr();

    this->_update_fifty_moves_ctr(move._attacker_type == Pieces::Pawn or move._defender_type != 255);

    if (move._attacker_type == Pieces::Pawn or move._defender_type != 255) this->_repetition_history.clear();
    this->_repetition_history.add_position(this->_hash);
}
void Position::_add_piece(uint8_t square, uint8_t type, uint8_t side) {
    if (!BitboardOperations::get_bit(this->_pieces._piece_bitboards[side][type], square)) {
        BitboardOperations::set_1(this->_pieces._piece_bitboards[side][type], square);
        this->_hash.invert_piece(square, type, side);
    }
}
void Position::_remove_piece(uint8_t square, uint8_t type, uint8_t side) {
    if (BitboardOperations::get_bit(this->_pieces._piece_bitboards[side][type], square)) {
        BitboardOperations::set_0(this->_pieces._piece_bitboards[side][type], square);
        this->_hash.invert_piece(square, type, side);
    }
}
void Position::_change_en_passant(uint8_t en_passant) {
    this->_en_passant = en_passant;
}
void Position::_remove_w_l_castling() {
    if (this->_w_l_castling) {
        this->_w_l_castling = false;
        this->_hash.invert_w_l_castling();
    }
}
void Position::_remove_w_s_castling() {
    if (this->_w_s_castling) {
        this->_w_s_castling = false;
        this->_hash.invert_w_s_castling();
    }
}
void Position::_remove_b_l_castling() {
    if (this->_b_l_castling) {
        this->_b_l_castling = false;
        this->_hash.invert_b_l_castling();
    }
}
void Position::_remove_b_s_castling() {
    if (this->_b_s_castling) {
        this->_b_s_castling = false;
        this->_hash.invert_b_s_castling();
    }
}
void Position::_update_move_ctr() {
    this->_move_ctr = this->_move_ctr + 0.5f;
    this->_hash.invert_move();
}
void Position::_update_fifty_moves_ctr(bool break_event) {
    if (break_event) this->_fifty_moves_ctr = 0;
    else this->_fifty_moves_ctr = this->_fifty_moves_ctr + 0.5f;
}