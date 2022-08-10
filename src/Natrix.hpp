#include "Engine/AI/AI.hpp"
#include "UI/UICore.hpp"


#pragma once


class Natrix {
public:
    Natrix();
    void start();
private:
    Position _position;
    AI _ai;

    uint8_t _sx0;
    uint8_t _sy0;
    uint8_t _sx1;
    uint8_t _sy1;
    uint8_t _sx2;
    uint8_t _sy2;

    uint8_t _random_side;

    float _brightness;

    UICore _ui;

    void _menu();

    void _game(uint8_t human_side, uint8_t ai_side);

    void _draw_game(bool rotated);
    void _draw_game_background();
    void _draw_game_board(bool rotated);
    void _draw_game_extra_information();
    void _draw_game_button();
    void _change_game_brightness();

    bool _white_victory();
    bool _black_victory();
    bool _draw();
    bool _game_finished();

    [[nodiscard]] bool _white_move() const;
    bool _black_move();

    static constexpr std::string_view Root = "../data/";

    struct PlayAsWhiteButton {
        static constexpr int32_t X = 40;
        static constexpr int32_t Y = 200;

        static constexpr int32_t Width = 128;
        static constexpr int32_t Height = 128;

        static constexpr std::string_view TextureName = "white_knight";
    };
    struct PlayAsBlackButton {
        static constexpr int32_t X = 40;
        static constexpr int32_t Y = 350;

        static constexpr int32_t Width = 128;
        static constexpr int32_t Height = 128;

        static constexpr std::string_view TextureName = "black_knight";
    };
    struct PlayAsRandomButton {
        static constexpr int32_t X = 40;
        static constexpr int32_t Y = 500;

        static constexpr int32_t Width = 128;
        static constexpr int32_t Height = 128;

        static constexpr std::string_view TextureName = "half_knight";
    };

    struct ReturnToMenuButton {
        static constexpr int32_t X = 1500;
        static constexpr int32_t Y = 20;

        static constexpr int32_t Width = 400;
        static constexpr int32_t Height = 100;

        static constexpr std::string_view Label = "Вернуться в меню";
    };

    static constexpr float BrightnessChangePerFrame = 1.0f / 60.0f * 4.0f;
};