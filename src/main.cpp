#define TEST_LEGAL_MOVE_GENERATION false
#define TEST_AI false
#define SHOW_UI true


#if TEST_LEGAL_MOVE_GENERATION
    #include "Engine/Base/MoveGeneration/LegalMoveGenTester.hpp"
#endif


#if TEST_AI
    #include "Engine/AI/AI.hpp"
#endif


#if SHOW_UI
    #include "Natrix.hpp"
#endif


int main() {
    #if TEST_LEGAL_MOVE_GENERATION
        LegalMoveGenTester::test();
    #endif


    #if TEST_AI
        Position position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};
        AI ai;
        ai.best_move(position, Pieces::White, 0, 60000);
    #endif


    #if SHOW_UI
        Natrix natrix;
        natrix.start();
    #endif


    return 0;
}