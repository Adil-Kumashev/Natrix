#include "Pieces.hpp"


Pieces::Pieces() = default;
Pieces::Pieces(const std::string& short_fen) {
    uint8_t x = 0;
    uint8_t y = 7;

    uint8_t side;

    for (auto buff : short_fen) {
        if (buff == '/') {
            x = 0;
            y = y - 1;
        }
        else if (std::isdigit(buff)) {
            x = x + buff - '0';
        }
        else {
            if (std::isupper(buff)) {
                buff = std::tolower(buff);
                side = Pieces::White;
            }
            else side = Pieces::Black;

            switch (buff) {
                case 'p': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::Pawn], y * 8 + x); break;
                case 'n': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::Knight], y * 8 + x); break;
                case 'b': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::Bishop], y * 8 + x); break;
                case 'r': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::Rook], y * 8 + x); break;
                case 'q': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::Queen], y * 8 + x); break;
                case 'k': BitboardOperations::set_1(this->_piece_bitboards[side][Pieces::King], y * 8 + x); break;
            }

            x = x + 1;
        }
    }

    this->update_bitboards();
}
std::ostream &operator<<(std::ostream &ostream, Pieces pieces) {
    ostream << ANSI::Green;

    for (int8_t y = 7; y >= 0; y = y - 1) {
        for (uint8_t x = 0; x < 8; x = x + 1) {
            ostream << "|  ";

            if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], y * 8 + x)) ostream << "♙";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Knight], y * 8 + x)) ostream << "♘";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Bishop], y * 8 + x)) ostream << "♗";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Rook], y * 8 + x)) ostream << "♖";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Queen], y * 8 + x)) ostream << "♕";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::King], y * 8 + x)) ostream << "♔";

            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], y * 8 + x)) ostream << "♟";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Knight], y * 8 + x)) ostream << "♞";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Bishop], y * 8 + x)) ostream << "♝";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Rook], y * 8 + x)) ostream << "♜";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Queen], y * 8 + x)) ostream << "♛";
            else if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::King], y * 8 + x)) ostream << "♚";

            else ostream << " ";

            ostream << "  ";
        }
        ostream << "|\n";
    }

    ostream << ANSI::End;

    return ostream;
}
bool operator ==(Pieces left, Pieces right) {
    for (uint8_t i = 0; i < 2; i = i + 1) {
        for (uint8_t j = 0; j < 6; j = j + 1) {
            if (left._piece_bitboards[i][j] != right._piece_bitboards[i][j]) return false;
        }
    }

    return true;
}
void Pieces::update_bitboards() {
    this->_side_bitboards[Pieces::White] = this->_piece_bitboards[Pieces::White][Pieces::Pawn] |
                                           this->_piece_bitboards[Pieces::White][Pieces::Knight] |
                                           this->_piece_bitboards[Pieces::White][Pieces::Bishop] |
                                           this->_piece_bitboards[Pieces::White][Pieces::Rook] |
                                           this->_piece_bitboards[Pieces::White][Pieces::Queen] |
                                           this->_piece_bitboards[Pieces::White][Pieces::King];

    this->_side_bitboards[Pieces::Black] = this->_piece_bitboards[Pieces::Black][Pieces::Pawn] |
                                           this->_piece_bitboards[Pieces::Black][Pieces::Knight] |
                                           this->_piece_bitboards[Pieces::Black][Pieces::Bishop] |
                                           this->_piece_bitboards[Pieces::Black][Pieces::Rook] |
                                           this->_piece_bitboards[Pieces::Black][Pieces::Queen] |
                                           this->_piece_bitboards[Pieces::Black][Pieces::King];

    this->_inversion_side_bitboards[Pieces::White] = ~this->_side_bitboards[Pieces::White];
    this->_inversion_side_bitboards[Pieces::Black] = ~this->_side_bitboards[Pieces::Black];

    this->_all = this->_side_bitboards[Pieces::White] | this->_side_bitboards[Pieces::Black];
    this->_empty = ~this->_all;
}
uint8_t Pieces::inverse(uint8_t side) {
    return !side;
}