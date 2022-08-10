#include "../Base/MoveGeneration/PsLegalMoveMaskGen.hpp"
#include "PassedPawnMasks.hpp"
#include "PawnShieldMasks.hpp"


#pragma once


class StaticEvaluator {
public:
    static int32_t evaluate(Pieces pieces, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling, bool white_castling_happened, bool black_castling_happened, bool show_debug_info = false);
private:
    static int32_t _material(Pieces pieces);
    static int32_t _mobility(Pieces pieces);
    static int32_t _pawn_structure_double_pawn(Pieces pieces);
    static int32_t _pawn_structure_connected_pawn(Pieces pieces);
    static int32_t _pawn_structure_pawn_promotion(Pieces pieces);
    static int32_t _king_safety_crashed_castling(bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling, bool white_castling_happened, bool black_castling_happened);
    static int32_t _king_safety_pawn_shield(Pieces pieces, bool white_castling_happened, bool black_castling_happened);
    static int32_t _two_bishops(Pieces pieces);
    static int32_t _endgame(Pieces pieces, bool white_leading);

    struct Material {
        static constexpr int32_t Pawn = 100;
        static constexpr int32_t Knight = 305;
        static constexpr int32_t Bishop = 333;
        static constexpr int32_t Rook = 563;
        static constexpr int32_t Queen = 950;
    };
    struct Mobility {
        static constexpr int32_t Knight = 9;
        static constexpr int32_t Bishop = 4;
        static constexpr int32_t Rook = 3;
        static constexpr int32_t Queen = 3;
    };
    struct PawnStructure {
        static constexpr int32_t DoublePawn = -25;
        static constexpr int32_t ConnectedPawn = 12;
        static constexpr std::array<int32_t, 8> DefaultPawnPromotion = {0, 0, 0, 0, 10, 20, 30, 0};
        static constexpr std::array<int32_t, 8> PassedPawnPromotion = {0, 50, 50, 50, 70, 90, 110, 0};
    };
    struct KingSafety {
        static constexpr int32_t CrashedCastling = -50;
        static constexpr int32_t PawnShield = 33;
    };
    static constexpr int32_t TwoBishops = 50;
    struct Endgame {
        static constexpr int32_t MaximumPiecesForEndgame = 8;
        static constexpr int32_t AttackerKingProximityToDefenderKing = 10;
        static constexpr int32_t DistanceBetweenDefenderKingAndMiddle = 10;
    };

    friend class MoveSorter;
};