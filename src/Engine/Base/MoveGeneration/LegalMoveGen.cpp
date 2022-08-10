#include "LegalMoveGen.hpp"


MoveList LegalMoveGen::generate(Position position, uint8_t side, bool only_captures) {
    /* TODO: Постараться оптимизировать функцию.
     * На моем ноутбуке генерация ходов без проверки на легальность в начальной позиции на глубине 6 работает со скоростью 30 MNPS,
     * когда как с включенной проверкой - со скоростью 6 MNPS. Возможно, стоит подумать не только об оптимизации проверки
     * хода на легальность, но и об оптимизации генерации перемещений скользящих фигур, ведь долгая проверка на легальность
     * наивным алгоритмом - следствие долгой генерации ходов. Запись от 28 июля.
     * */

    MoveList moves;

    Bitboard pawn_left_captures_mask = PsLegalMoveMaskGen::generate_pawn_left_captures_mask(position._pieces, side, false);
    Bitboard pawn_right_captures_mask = PsLegalMoveMaskGen::generate_pawn_right_captures_mask(position._pieces, side, false);

    int8_t pawn_left_capture;
    int8_t pawn_right_capture;

    if (side == Pieces::White) {
        pawn_left_capture = -7;
        pawn_right_capture = -9;
    }
    else {
        pawn_left_capture = 9;
        pawn_right_capture = 7;
    }

    LegalMoveGen::_pawn_mask_to_moves(position._pieces, pawn_left_captures_mask, side, pawn_left_capture, true,Move::Flag::Default, moves);
    LegalMoveGen::_pawn_mask_to_moves(position._pieces, pawn_right_captures_mask, side, pawn_right_capture, true,Move::Flag::Default, moves);

    if (!only_captures) {
        Bitboard pawn_default_mask = PsLegalMoveMaskGen::generate_pawn_default_mask(position._pieces, side);
        Bitboard pawn_long_mask = PsLegalMoveMaskGen::generate_pawn_long_mask(position._pieces, side);

        int8_t pawn_default_move;
        int8_t pawn_long_move;

        if (side == Pieces::White) {
            pawn_default_move = -8;
            pawn_long_move = -16;
        }
        else {
            pawn_default_move = 8;
            pawn_long_move = 16;
        }

        LegalMoveGen::_pawn_mask_to_moves(position._pieces, pawn_default_mask, side, pawn_default_move, false,Move::Flag::Default, moves);
        LegalMoveGen::_pawn_mask_to_moves(position._pieces, pawn_long_mask, side, pawn_long_move, false,Move::Flag::PawnLongMove, moves);
    }

    Bitboard all_knights = position._pieces._piece_bitboards[side][Pieces::Knight];
    Bitboard all_bishops = position._pieces._piece_bitboards[side][Pieces::Bishop];
    Bitboard all_rooks = position._pieces._piece_bitboards[side][Pieces::Rook];
    Bitboard all_queens = position._pieces._piece_bitboards[side][Pieces::Queen];

    uint8_t attacker_p;
    Bitboard mask;

    while (all_knights) {
        attacker_p = BitboardOperations::bsf(all_knights);
        BitboardOperations::set_0(all_knights, attacker_p);
        mask = PsLegalMoveMaskGen::generate_knight_mask(position._pieces, attacker_p, side, only_captures);
        LegalMoveGen::_piece_mask_to_moves(position._pieces, mask, attacker_p, Pieces::Knight, side, moves);
    }
    while (all_bishops) {
        attacker_p = BitboardOperations::bsf(all_bishops);
        BitboardOperations::set_0(all_bishops, attacker_p);
        mask = PsLegalMoveMaskGen::generate_bishop_mask(position._pieces, attacker_p, side, only_captures);
        LegalMoveGen::_piece_mask_to_moves(position._pieces, mask, attacker_p, Pieces::Bishop, side, moves);
    }
    while (all_rooks) {
        attacker_p = BitboardOperations::bsf(all_rooks);
        BitboardOperations::set_0(all_rooks, attacker_p);
        mask = PsLegalMoveMaskGen::generate_rook_mask(position._pieces, attacker_p, side, only_captures);
        LegalMoveGen::_piece_mask_to_moves(position._pieces, mask, attacker_p, Pieces::Rook, side, moves);
    }
    while (all_queens) {
        attacker_p = BitboardOperations::bsf(all_queens);
        BitboardOperations::set_0(all_queens, attacker_p);
        mask = PsLegalMoveMaskGen::generate_queen_mask(position._pieces, attacker_p, side, only_captures);
        LegalMoveGen::_piece_mask_to_moves(position._pieces, mask, attacker_p, Pieces::Queen, side, moves);
    }

    attacker_p = BitboardOperations::bsf(position._pieces._piece_bitboards[side][Pieces::King]);
    mask = PsLegalMoveMaskGen::generate_king_mask(position._pieces, attacker_p, side, only_captures);
    LegalMoveGen::_piece_mask_to_moves(position._pieces, mask, attacker_p, Pieces::King, side, moves);

    LegalMoveGen::_add_en_passant_captures(position._pieces, side, position._en_passant, moves);
    if (!only_captures) {
        if (side == Pieces::White) LegalMoveGen::_add_castling_moves(position._pieces, Pieces::White, position._w_l_castling,position._w_s_castling, moves);
        else LegalMoveGen::_add_castling_moves(position._pieces, Pieces::Black, position._b_l_castling,position._b_s_castling, moves);
    }

    return moves;
}
void LegalMoveGen::_piece_mask_to_moves(Pieces pieces, Bitboard mask, uint8_t attacker_p, uint8_t attacker_type, uint8_t attacker_side, MoveList &moves) {
    uint8_t defender_p;
    uint8_t defender_type;

    Move move;

    while (mask) {
        defender_p = BitboardOperations::bsf(mask);
        BitboardOperations::set_0(mask, defender_p);

        defender_type = 255;
        for (uint8_t i = 0; i < 6; i = i + 1) {
            if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::inverse(attacker_side)][i], defender_p)) {
                defender_type = i;
                break;
            }
        }

        move = {attacker_p, defender_p, attacker_type, attacker_side, defender_type, Pieces::inverse(attacker_side)};

        if (LegalMoveGen::_is_legal(pieces, move, false)) moves.push_back(move);
    }
}
void LegalMoveGen::_pawn_mask_to_moves(Pieces pieces, Bitboard mask, uint8_t attacker_side, int8_t attacker_index, bool look_for_defender, uint8_t flag, MoveList &moves) {
    uint8_t defender_p;
    uint8_t defender_type = 255;

    Move move;

    while (mask) {
        defender_p = BitboardOperations::bsf(mask);
        BitboardOperations::set_0(mask, defender_p);

        if (look_for_defender) {
            defender_type = 255;
            for (uint8_t i = 0; i < 6; i = i + 1) {
                if (BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::inverse(attacker_side)][i], defender_p)) {
                    defender_type = i;
                    break;
                }
            }
        }

        move = {(uint8_t)(defender_p + attacker_index), defender_p, Pieces::Pawn, attacker_side, defender_type, Pieces::inverse(attacker_side), flag};

        if (LegalMoveGen::_is_legal(pieces, move, false)) {
            if (defender_p < 8 or defender_p > 55) {
                moves.push_back({(uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToKnight});
                moves.push_back({(uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToBishop});
                moves.push_back({(uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToRook});
                moves.push_back({(uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToQueen});
            }
            else moves.push_back(move);
        }
    }
}
bool LegalMoveGen::_is_legal(Pieces pieces, Move move, bool en_passant_capture) {
    BitboardOperations::set_0(pieces._piece_bitboards[move._attacker_side][move._attacker_type], move._from);
    BitboardOperations::set_1(pieces._piece_bitboards[move._attacker_side][move._attacker_type], move._to);
    if (move._defender_type != 255) BitboardOperations::set_0(pieces._piece_bitboards[move._defender_side][move._defender_type], move._to);
    if (en_passant_capture) {
        if (move._attacker_side == Pieces::White) BitboardOperations::set_0(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], move._to - 8);
        BitboardOperations::set_0(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], move._to + 8);
    }

    pieces.update_bitboards();

    if (PsLegalMoveMaskGen::in_danger(pieces, BitboardOperations::bsf(pieces._piece_bitboards[move._attacker_side][Pieces::King]), move._attacker_side)) return false;

    return true;
}
void LegalMoveGen::_add_en_passant_captures(Pieces pieces, uint8_t side, uint8_t en_passant, MoveList &moves) {
    if (en_passant == 255) return;

    Move move;

    if (side == Pieces::White) {
        if (en_passant % 8 != 7 and BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], en_passant - 7)) {
            move = {(uint8_t)(en_passant - 7), en_passant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture};
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
        if (en_passant % 8 != 0 and BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], en_passant - 9)) {
            move = {(uint8_t)(en_passant - 9), en_passant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture};
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
    }
    else {
        if (en_passant % 8 != 0 and BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], en_passant + 7)) {
            move = {(uint8_t)(en_passant + 7), en_passant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture};
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
        if (en_passant % 8 != 7 and BitboardOperations::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], en_passant + 9)) {
            move = {(uint8_t)(en_passant + 9), en_passant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture};
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
    }
}
void LegalMoveGen::_add_castling_moves(Pieces pieces, uint8_t side, bool long_castling, bool short_castling, MoveList &moves) {
    uint8_t index;
    uint8_t long_castling_flag;
    uint8_t short_castling_flag;
    if (side == Pieces::White) {
        index = 0;
        long_castling_flag = Move::Flag::WhiteLongCastling;
        short_castling_flag = Move::Flag::WhiteShortCastling;
    }
    else {
        index = 56;
        long_castling_flag = Move::Flag::BlackLongCastling;
        short_castling_flag = Move::Flag::BlackShortCastling;
    }

    if (long_castling and BitboardOperations::get_bit(pieces._piece_bitboards[side][Pieces::Rook], 0 + index) and BitboardOperations::get_bit(pieces._empty, 1 + index) and BitboardOperations::get_bit(pieces._empty, 2 + index) and BitboardOperations::get_bit(pieces._empty, 3 + index)) {
        if (!PsLegalMoveMaskGen::in_danger(pieces, BitboardOperations::bsf(pieces._piece_bitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::in_danger(pieces, 2 + index, side) and !PsLegalMoveMaskGen::in_danger(pieces, 3 + index, side)) moves.push_back({(uint8_t)(4 + index), (uint8_t)(2 + index), Pieces::King, side, 255, 255, long_castling_flag});
    }
    if (short_castling and BitboardOperations::get_bit(pieces._piece_bitboards[side][Pieces::Rook], 7 + index) and BitboardOperations::get_bit(pieces._empty, 5 + index) and BitboardOperations::get_bit(pieces._empty, 6 + index)) {
        if (!PsLegalMoveMaskGen::in_danger(pieces, BitboardOperations::bsf(pieces._piece_bitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::in_danger(pieces, 5 + index, side) and !PsLegalMoveMaskGen::in_danger(pieces, 6 + index, side)) moves.push_back({(uint8_t)(4 + index), (uint8_t)(6 + index), Pieces::King, side, 255, 255, short_castling_flag});
    }
}