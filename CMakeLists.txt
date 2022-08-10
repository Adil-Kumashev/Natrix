cmake_minimum_required(VERSION 3.22)
project(Natrix)
set(CMAKE_CXX_STANDARD 23)
add_executable(Natrix ../src/main.cpp

        ../src/ANSI.hpp

        ../src/Engine/Base/PositionRepresentation/Bitboard.hpp
        ../src/Engine/Base/PositionRepresentation/Pieces.hpp ../src/Engine/Base/PositionRepresentation/Pieces.cpp
        ../src/Engine/Base/PositionRepresentation/ZobristHashConstants.hpp
        ../src/Engine/Base/PositionRepresentation/ZobristHash.hpp ../src/Engine/Base/PositionRepresentation/ZobristHash.cpp
        ../src/Engine/Base/PositionRepresentation/RepetitionHistory.hpp ../src/Engine/Base/PositionRepresentation/RepetitionHistory.cpp
        ../src/Engine/Base/PositionRepresentation/Move.hpp ../src/Engine/Base/PositionRepresentation/Move.cpp
        ../src/Engine/Base/PositionRepresentation/Position.hpp ../src/Engine/Base/PositionRepresentation/Position.cpp

        ../src/Engine/Base/MoveGeneration/KnightMasks.hpp
        ../src/Engine/Base/MoveGeneration/KingMasks.hpp
        ../src/Engine/Base/MoveGeneration/SlidersMasks.hpp
        ../src/Engine/Base/MoveGeneration/PsLegalMoveMaskGen.hpp ../src/Engine/Base/MoveGeneration/PsLegalMoveMaskGen.cpp
        ../src/Engine/Base/MoveGeneration/MoveList.hpp ../src/Engine/Base/MoveGeneration/MoveList.cpp
        ../src/Engine/Base/MoveGeneration/LegalMoveGen.hpp ../src/Engine/Base/MoveGeneration/LegalMoveGen.cpp
        ../src/Engine/Base/MoveGeneration/LegalMoveGenTester.hpp ../src/Engine/Base/MoveGeneration/LegalMoveGenTester.cpp

        ../src/Engine/AI/PassedPawnMasks.hpp
        ../src/Engine/AI/PawnShieldMasks.hpp
        ../src/Engine/AI/StaticEvaluator.hpp ../src/Engine/AI/StaticEvaluator.cpp
        ../src/Engine/AI/MoveSorter.hpp ../src/Engine/AI/MoveSorter.cpp
        ../src/Engine/AI/Entry.hpp ../src/Engine/AI/Entry.cpp
        ../src/Engine/AI/TranspositionTable.hpp ../src/Engine/AI/TranspositionTable.cpp
        ../src/Engine/AI/OpeningBook.hpp ../src/Engine/AI/OpeningBook.cpp
        ../src/Engine/AI/AI.hpp ../src/Engine/AI/AI.cpp

        ../src/UI/UICore.hpp ../src/UI/UICore.cpp

        ../src/Natrix.hpp ../src/Natrix.cpp)
include(FindPkgConfig)
add_definitions(-Werror)
add_definitions(-O3)
add_definitions(-flto)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# Подключение потоков.
find_package(Threads REQUIRED)
target_link_libraries(Natrix Threads::Threads)


# Подключение SDL2 и дополнений.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(Natrix ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})