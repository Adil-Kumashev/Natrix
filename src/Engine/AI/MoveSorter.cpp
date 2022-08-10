#include "MoveSorter.hpp"


MoveList MoveSorter::sort(Pieces pieces, MoveList moves) {
    for (uint8_t i = 0; i < moves.size() - 1; i = i + 1) {
        for (uint8_t j = 0; j < moves.size() - i - 1; j = j + 1) {
            if (MoveSorter::_evaluate_move(pieces, moves[j]) < MoveSorter::_evaluate_move(pieces, moves[j + 1])) std::swap(moves[j], moves[j + 1]);
        }
    }

    return moves;
}
int32_t MoveSorter::_evaluate_move(Pieces pieces, Move move) {
    int32_t evaluation = 0;

    if (move._attacker_type != Pieces::Pawn) {
        Bitboard opponent_pawn_attacks = PsLegalMoveMaskGen::generate_pawn_left_captures_mask(pieces, Pieces::inverse(move._attacker_side), true) | PsLegalMoveMaskGen::generate_pawn_right_captures_mask(pieces, Pieces::inverse(move._attacker_side), true);
        if (BitboardOperations::get_bit(opponent_pawn_attacks, move._to)) {
            switch (move._attacker_type) {
                case Pieces::Knight: evaluation = evaluation - StaticEvaluator::Material::Knight; break;
                case Pieces::Bishop: evaluation = evaluation - StaticEvaluator::Material::Bishop; break;
                case Pieces::Rook: evaluation = evaluation - StaticEvaluator::Material::Rook; break;
                case Pieces::Queen: evaluation = evaluation - StaticEvaluator::Material::Queen; break;
                // Король не может быть на поле, битое пешкой противника.
            }
        }
    }

    if (move._defender_type != 255) {
        switch (move._defender_type) {
            case Pieces::Pawn: evaluation = evaluation + 1000 * StaticEvaluator::Material::Pawn; break;
            case Pieces::Knight: evaluation = evaluation + 1000 * StaticEvaluator::Material::Knight; break;
            case Pieces::Bishop: evaluation = evaluation + 1000 * StaticEvaluator::Material::Bishop; break;
            case Pieces::Rook: evaluation = evaluation + 1000 * StaticEvaluator::Material::Rook; break;
            case Pieces::Queen: evaluation = evaluation + 1000 * StaticEvaluator::Material::Queen; break;
            // Короля нельзя съесть.
        }
        switch (move._attacker_type) {
            case Pieces::Pawn: evaluation = evaluation - StaticEvaluator::Material::Pawn; break;
            case Pieces::Knight: evaluation = evaluation - StaticEvaluator::Material::Knight; break;
            case Pieces::Bishop: evaluation = evaluation - StaticEvaluator::Material::Bishop; break;
            case Pieces::Rook: evaluation = evaluation - StaticEvaluator::Material::Rook; break;
            case Pieces::Queen: evaluation = evaluation - StaticEvaluator::Material::Queen; break;
            // Если съедает король, то ничего не вычитаем, ведь короля нельзя съесть, следовательно, никакого ответного взятия, вероятно, не будет.
        }
    }

    return evaluation;
}