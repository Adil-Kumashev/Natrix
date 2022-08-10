#include "Natrix.hpp"


Natrix::Natrix() {
    this->_ai = {(std::string)Natrix::Root + "AI/opening_book.txt"};

    this->_random_side = time(nullptr) % 2;

    this->_brightness = 0;

    this->_ui = {SDL_INIT_EVERYTHING, IMG_INIT_PNG, MIX_INIT_MP3, 44100, MIX_DEFAULT_FORMAT, 1, 2048};

    int32_t display_x, display_y;
    std::tie(display_x, display_y) = UICore::get_display_resolution();

    this->_ui.set_window(SDL_WINDOW_FULLSCREEN, 0, 0, display_x, display_y, "Natrix", (std::string)Natrix::Root + "UI/images/logotype.png");
    this->_ui.set_renderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, (float)display_x / 1920.0f, (float)display_y / 1080.0f);

    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/pawn.png", "white_pawn");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/knight.png", "white_knight");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/bishop.png", "white_bishop");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/rook.png", "white_rook");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/queen.png", "white_queen");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/white/king.png", "white_king");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/pawn.png", "black_pawn");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/knight.png", "black_knight");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/bishop.png", "black_bishop");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/rook.png", "black_rook");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/queen.png", "black_queen");
    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/black/king.png", "black_king");

    this->_ui.load_texture((std::string)Natrix::Root + "UI/images/chessmen/half_knight.png", "half_knight");

    this->_ui.load_sound((std::string)Natrix::Root + "UI/sounds/click.mp3", "click");
    this->_ui.load_sound((std::string)Natrix::Root + "UI/sounds/message.mp3", "message");
    this->_ui.load_sound((std::string)Natrix::Root + "UI/sounds/move.mp3", "move");
    this->_ui.load_sound((std::string)Natrix::Root + "UI/sounds/capture.mp3", "capture");

    this->_ui.load_font((std::string)Natrix::Root + "UI/fonts/cyrillicoldface.ttf", 18, "font_small");
    this->_ui.load_font((std::string)Natrix::Root + "UI/fonts/cyrillicoldface.ttf", 41, "font_large");
}
void Natrix::start() {
    this->_menu();
}
void Natrix::_menu() {
    SDL_Event event;

    int32_t mouse_x, mouse_y;

    uint8_t human_side;

    bool up_brightness = true;
    bool down_brightness = false;

    for (; ;) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) this->_ui.quit(0);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN and !down_brightness and !up_brightness) {
                std::tie(mouse_x, mouse_y) = this->_ui.get_mouse_position();

                if (mouse_x >= Natrix::PlayAsWhiteButton::X and mouse_x < Natrix::PlayAsWhiteButton::X + Natrix::PlayAsWhiteButton::Width and mouse_y >= Natrix::PlayAsWhiteButton::Y and mouse_y < Natrix::PlayAsWhiteButton::Y + Natrix::PlayAsWhiteButton::Height) {
                    down_brightness = true;
                    human_side = Pieces::White;
                    this->_ui.play_sound("click", 0, 0);
                }
                else if (mouse_x >= Natrix::PlayAsBlackButton::X and mouse_x < Natrix::PlayAsBlackButton::X + Natrix::PlayAsBlackButton::Width and mouse_y >= Natrix::PlayAsBlackButton::Y and mouse_y < Natrix::PlayAsBlackButton::Y + Natrix::PlayAsBlackButton::Height) {
                    down_brightness = true;
                    human_side = Pieces::Black;
                    this->_ui.play_sound("click", 0, 0);
                }
                else if (mouse_x >= Natrix::PlayAsRandomButton::X and mouse_x < Natrix::PlayAsRandomButton::X + Natrix::PlayAsRandomButton::Width and mouse_y >= Natrix::PlayAsRandomButton::Y and mouse_y < Natrix::PlayAsRandomButton::Y + Natrix::PlayAsRandomButton::Height) {
                    down_brightness = true;
                    human_side = this->_random_side;
                    this->_random_side = Pieces::inverse(this->_random_side);
                    this->_ui.play_sound("click", 0, 0);
                }
            }
        }

        this->_ui.clear();
        this->_ui.draw_rectangle(0, 0, 1920, 1080, 22, 21, 18, 255);
        this->_ui.draw_rectangle(20, 20, 1600, 650, 0, 0, 0, 255 / 4);
        this->_ui.draw_text("font_large", "Выберите сторону", 65, 40, 255, 255, 255, 255);
        this->_ui.draw_texture((std::string) Natrix::PlayAsWhiteButton::TextureName, Natrix::PlayAsWhiteButton::X, Natrix::PlayAsWhiteButton::Y, false, 1, 1);
        this->_ui.draw_texture((std::string) Natrix::PlayAsBlackButton::TextureName, Natrix::PlayAsBlackButton::X, Natrix::PlayAsBlackButton::Y, false, 1, 1);
        this->_ui.draw_texture((std::string) Natrix::PlayAsRandomButton::TextureName, Natrix::PlayAsRandomButton::X, Natrix::PlayAsRandomButton::Y, false, 1, 1);
        this->_ui.draw_text("font_small", "Белые", Natrix::PlayAsWhiteButton::X + 45, Natrix::PlayAsWhiteButton::Y + 120, 255, 255, 255, 255);
        this->_ui.draw_text("font_small", "Черные", Natrix::PlayAsBlackButton::X + 45, Natrix::PlayAsBlackButton::Y + 120, 255, 255, 255, 255);
        this->_ui.draw_text("font_small", "Случайно", Natrix::PlayAsRandomButton::X + 35, Natrix::PlayAsRandomButton::Y + 120, 255, 255, 255, 255);
        this->_ui.draw_rectangle(0, 0, 1920, 1080, 0, 0, 0, (int) (255 * (1 - this->_brightness)));
        this->_ui.update();

        if (up_brightness) {
            this->_brightness = this->_brightness + Natrix::BrightnessChangePerFrame;
            if (this->_brightness > 1) {
                this->_brightness = 1;
                up_brightness = false;
            }
        }
        if (down_brightness) {
            this->_brightness = this->_brightness - Natrix::BrightnessChangePerFrame;
            if (this->_brightness < 0) {
                this->_brightness = 0;
                this->_game(human_side, Pieces::inverse(human_side));
                break;
            }
        }
    }
}
void Natrix::_game(uint8_t human_side, uint8_t ai_side) {
    this->_position = {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1};

    this->_sx0 = 255;
    this->_sy0 = 255;
    this->_sx1 = 255;
    this->_sy1 = 255;
    this->_sx2 = 255;
    this->_sy2 = 255;

    MoveList moves;
    Move move;

    SDL_Event event;

    int32_t mouse_x, mouse_y;
    uint8_t buff_x, buff_y, buff_p;
    bool high_row, right_column;

    bool up_brightness = true;
    bool down_brightness = false;

    for (;;) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) this->_ui.quit(0);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN and !up_brightness and !down_brightness) {
                std::tie(mouse_x, mouse_y) = this->_ui.get_mouse_position();

                if (mouse_x >= Natrix::ReturnToMenuButton::X and mouse_x < Natrix::ReturnToMenuButton::X + Natrix::ReturnToMenuButton::Width and mouse_y >= Natrix::ReturnToMenuButton::Y and mouse_y < Natrix::ReturnToMenuButton::Y + Natrix::ReturnToMenuButton::Height) {
                    down_brightness = true;
                    this->_ui.play_sound("click", 0, 0);
                }
                else if (!this->_game_finished()) {
                    if (mouse_x < 20 or mouse_x > 1044 or mouse_y < 36 or mouse_y > 1060) {
                        buff_x = 255;
                        buff_y = 255;
                    }
                    else {
                        buff_x = (mouse_x - 20) / 128;
                        buff_y = 7 - (mouse_y - 36) / 128;
                        if (ai_side == Pieces::White) {
                            buff_x = 7 - buff_x;
                            buff_y = 7 - buff_y;
                        }

                        high_row = (((mouse_y - 36) % 128) < 64);
                        right_column = (((mouse_x - 20) % 128) >= 64);

                        if (high_row) {
                            if (right_column) buff_p = Move::Flag::PromoteToBishop;
                            else buff_p = Move::Flag::PromoteToKnight;
                        }
                        else {
                            if (right_column) buff_p = Move::Flag::PromoteToQueen;
                            else buff_p = Move::Flag::PromoteToRook;
                        }

                        if (buff_x == this->_sx2 and buff_y == this->_sy2) {
                            this->_sx2 = 255;
                            this->_sy2 = 255;
                            this->_ui.play_sound("move", 0, 0);
                        }
                        else if (BitboardOperations::get_bit(this->_position._pieces._side_bitboards[human_side],buff_y * 8 + buff_x)) {
                            this->_sx2 = buff_x;
                            this->_sy2 = buff_y;
                            this->_ui.play_sound("move", 0, 0);
                        }
                        else if (this->_sx2 != 255 and this->_sy2 != 255) {
                            moves = LegalMoveGen::generate(this->_position, human_side);

                            for (uint8_t i = 0; i < moves.size(); i = i + 1) {
                                move = moves[i];

                                if (move._from == this->_sy2 * 8 + this->_sx2 and move._to == buff_y * 8 + buff_x and ((move._flag != Move::Flag::PromoteToKnight and move._flag != Move::Flag::PromoteToBishop and move._flag != Move::Flag::PromoteToRook and move._flag != Move::Flag::PromoteToQueen) or move._flag == buff_p)) {
                                    this->_position.move(move);
                                    this->_sx0 = move._from % 8;
                                    this->_sy0 = move._from / 8;
                                    this->_sx1 = move._to % 8;
                                    this->_sy1 = move._to / 8;
                                    this->_sx2 = 255;
                                    this->_sy2 = 255;

                                    if (this->_game_finished()) this->_ui.play_sound("message", 0, 0);
                                    else {
                                        if (move._defender_type == 255 and move._flag != Move::Flag::EnPassantCapture) this->_ui.play_sound("move", 0, 0);
                                        else this->_ui.play_sound("capture", 0, 0);
                                        this->_draw_game(ai_side == Pieces::White);

                                        move = this->_ai.best_move(this->_position, ai_side, 250, 6 * 1e+3);

                                        UICore::remove_events();

                                        this->_position.move(move);
                                        this->_sx0 = move._from % 8;
                                        this->_sy0 = move._from / 8;
                                        this->_sx1 = move._to % 8;
                                        this->_sy1 = move._to / 8;
                                        this->_sx2 = 255;
                                        this->_sy2 = 255;

                                        if (this->_game_finished()) this->_ui.play_sound("message", 0, 0);
                                        else {
                                            if (move._defender_type == 255 and move._flag != Move::Flag::EnPassantCapture) this->_ui.play_sound("move", 0, 0);
                                            else this->_ui.play_sound("capture", 0, 0);
                                        }
                                    }

                                    break;
                                }
                            }
                        }

                        this->_draw_game(ai_side == Pieces::White);
                    }
                }
            }
        }

        if (up_brightness) {
            this->_brightness = this->_brightness + Natrix::BrightnessChangePerFrame;
            if (this->_brightness > 1) {
                this->_brightness = 1;
                up_brightness = false;
                if (ai_side == Pieces::White) {
                    move = this->_ai.best_move(this->_position, Pieces::White, 250, 6 * 1e+3);

                    UICore::remove_events();

                    this->_position.move(move);
                    this->_sx0 = move._from % 8;
                    this->_sy0 = move._from / 8;
                    this->_sx1 = move._to % 8;
                    this->_sy1 = move._to / 8;

                    this->_ui.play_sound("move", 0, 0);
                }
            }
            this->_draw_game(ai_side == Pieces::White);
        }
        if (down_brightness) {
            this->_brightness = this->_brightness - Natrix::BrightnessChangePerFrame;
            if (this->_brightness < 0) {
                this->_brightness = 0;
                this->_menu();
                break;
            }
            this->_draw_game(ai_side == Pieces::White);
        }

        SDL_Delay(20);
    }
}
void Natrix::_draw_game(bool rotated) {
    this->_ui.clear();

    this->_draw_game_background();
    this->_draw_game_board(rotated);
    this->_draw_game_extra_information();
    this->_draw_game_button();
    this->_change_game_brightness();

    this->_ui.update();
}
void Natrix::_draw_game_background() {
    this->_ui.draw_rectangle(0, 0, 1920, 1080, 22, 21, 18, 255);
}
void Natrix::_draw_game_board(bool rotated) {
    uint8_t side;
    if (this->_white_move()) side = Pieces::White;
    else side = Pieces::Black;

    MoveList moves = LegalMoveGen::generate(this->_position, side);
    Move move;

    std::array<std::tuple<uint8_t, uint8_t>, 64> shadows{};
    for (uint8_t i = 0; i < 64; i = i + 1) shadows[i] = std::make_tuple(255, 255);
    for (uint8_t i = 0; i < moves.size(); i = i + 1) {
        move = moves[i];
        if (move._from == this->_sy2 * 8 + this->_sx2) shadows[move._to] = std::make_tuple(move._attacker_side, move._attacker_type);
    }

    int32_t pos_x;
    int32_t pos_y;

    for (uint8_t x = 0; x < 8; x = x + 1) {
        for (uint8_t y = 0; y < 8; y = y + 1) {
            if (rotated) {
                pos_x = 20 + 128 * (7 - x);
                pos_y = 36 + 128 * y;
            }
            else {
                pos_x = 20 + 128 * x;
                pos_y = 36 + 128 * (7 - y);
            }

            if ((x == this->_sx0 and y == this->_sy0) or (x == this->_sx1 and y == this->_sy1) or (x == this->_sx2 and y == this->_sy2)) {
                if ((x + y) % 2 == 0) this->_ui.draw_rectangle(pos_x, pos_y, 128, 128, 171, 162, 58, 255);
                else this->_ui.draw_rectangle(pos_x, pos_y, 128, 128, 206, 210, 107, 255);
            }
            else {
                if ((x + y) % 2 == 0) this->_ui.draw_rectangle(pos_x, pos_y, 128, 128, 181, 136, 99, 255);
                else this->_ui.draw_rectangle(pos_x, pos_y, 128, 128, 240, 217, 181, 255);
            }

            if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::Pawn], y * 8 + x)) this->_ui.draw_texture("white_pawn", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::Knight], y * 8 + x)) this->_ui.draw_texture("white_knight", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::Bishop], y * 8 + x)) this->_ui.draw_texture("white_bishop", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::Rook], y * 8 + x)) this->_ui.draw_texture("white_rook", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::Queen], y * 8 + x)) this->_ui.draw_texture("white_queen", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::King], y * 8 + x)) this->_ui.draw_texture("white_king", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], y * 8 + x)) this->_ui.draw_texture("black_pawn", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::Knight], y * 8 + x)) this->_ui.draw_texture("black_knight", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::Bishop], y * 8 + x)) this->_ui.draw_texture("black_bishop", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::Rook], y * 8 + x)) this->_ui.draw_texture("black_rook", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::Queen], y * 8 + x)) this->_ui.draw_texture("black_queen", pos_x, pos_y, false, 1, 1);
            else if (BitboardOperations::get_bit(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::King], y * 8 + x)) this->_ui.draw_texture("black_king", pos_x, pos_y, false, 1, 1);

            if (y == 7 and shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Pawn)) {
                this->_ui.draw_texture("white_knight", pos_x, pos_y, false, 0.5, 0.5);
                this->_ui.draw_texture("white_bishop", pos_x + 64, pos_y, false, 0.5, 0.5);
                this->_ui.draw_texture("white_rook", pos_x, pos_y + 64, false, 0.5, 0.5);
                this->_ui.draw_texture("white_queen", pos_x + 64, pos_y + 64, false, 0.5, 0.5);
            }
            else if (y == 0 and shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Pawn)) {
                this->_ui.draw_texture("black_knight", pos_x, pos_y, false, 0.5, 0.5);
                this->_ui.draw_texture("black_bishop", pos_x + 64, pos_y, false, 0.5, 0.5);
                this->_ui.draw_texture("black_rook", pos_x, pos_y + 64, false, 0.5, 0.5);
                this->_ui.draw_texture("black_queen", pos_x + 64, pos_y + 64, false, 0.5, 0.5);
            }
            else {
                if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Pawn)) this->_ui.draw_texture("white_pawn", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Knight)) this->_ui.draw_texture("white_knight", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Bishop)) this->_ui.draw_texture("white_bishop", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Rook)) this->_ui.draw_texture("white_rook", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::Queen)) this->_ui.draw_texture("white_queen", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::White, Pieces::King)) this->_ui.draw_texture("white_king", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Pawn)) this->_ui.draw_texture("black_pawn", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Knight)) this->_ui.draw_texture("black_knight", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Bishop)) this->_ui.draw_texture("black_bishop", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Rook)) this->_ui.draw_texture("black_rook", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::Queen)) this->_ui.draw_texture("black_queen", pos_x, pos_y, false, 1, 0.5);
                else if (shadows[y * 8 + x] == std::make_tuple(Pieces::Black, Pieces::King)) this->_ui.draw_texture("black_king", pos_x, pos_y, false, 1, 0.5);
            }
        }
    }
}
void Natrix::_draw_game_extra_information() {
    if (this->_white_victory()) this->_ui.draw_text("font_large", "Победа белых", 1064, 36, 255, 255, 255, 255);
    else if (this->_black_victory()) this->_ui.draw_text("font_large", "Победа черных", 1064, 36, 255, 255, 255, 255);
    else if (this->_draw()) this->_ui.draw_text("font_large", "Ничья", 1064, 36, 255, 255, 255, 255);
    else if (this->_white_move()) this->_ui.draw_text("font_large", "Ход белых", 1064, 36, 255, 255, 255, 255);
    else if (this->_black_move()) this->_ui.draw_text("font_large", "Ход черных", 1064, 36, 255, 255, 255, 255);
}
void Natrix::_draw_game_button() {
    this->_ui.draw_rectangle(Natrix::ReturnToMenuButton::X, Natrix::ReturnToMenuButton::Y, Natrix::ReturnToMenuButton::Width, Natrix::ReturnToMenuButton::Height, 0, 0, 0, 255 / 2);
    this->_ui.draw_text("font_large", (std::string)Natrix::ReturnToMenuButton::Label, Natrix::ReturnToMenuButton::X + 20, Natrix::ReturnToMenuButton::Y + 20, 255, 255, 255, 255);
}
void Natrix::_change_game_brightness() {
    this->_ui.draw_rectangle(0, 0, 1920, 1080, 0, 0, 0, (int)(255 * (1 - this->_brightness)));
}
bool Natrix::_white_victory() {
    bool black_have_not_got_moves = (LegalMoveGen::generate(this->_position, Pieces::Black).size() == 0);
    bool black_in_check = PsLegalMoveMaskGen::in_danger(this->_position._pieces, BitboardOperations::bsf(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    return (black_have_not_got_moves and black_in_check);
}
bool Natrix::_black_victory() {
    bool white_have_not_got_moves = (LegalMoveGen::generate(this->_position, Pieces::White).size() == 0);
    bool white_in_check = PsLegalMoveMaskGen::in_danger(this->_position._pieces, BitboardOperations::bsf(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::King]), Pieces::White);

    return (white_have_not_got_moves and white_in_check);
}
bool Natrix::_draw() {
    bool white_have_not_got_moves = (LegalMoveGen::generate(this->_position, Pieces::White).size() == 0);
    bool white_in_check = PsLegalMoveMaskGen::in_danger(this->_position._pieces, BitboardOperations::bsf(this->_position._pieces._piece_bitboards[Pieces::White][Pieces::King]), Pieces::White);

    bool black_have_not_got_moves = (LegalMoveGen::generate(this->_position, Pieces::Black).size() == 0);
    bool black_in_check = PsLegalMoveMaskGen::in_danger(this->_position._pieces, BitboardOperations::bsf(this->_position._pieces._piece_bitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    bool fifty_moves_rule = (this->_position._fifty_moves_ctr >= 50);
    bool three_moves_rule = (this->_position._repetition_history.get_repetition_number(this->_position._hash) >= 3);

    bool white_move = this->_white_move();
    bool black_move = !white_move;

    if (white_have_not_got_moves and !white_in_check and white_move) return true;
    if (black_have_not_got_moves and !black_in_check and black_move) return true;
    if (fifty_moves_rule) return true;
    if (three_moves_rule) return true;

    return false;
}
bool Natrix::_game_finished() {
    return (this->_white_victory() or this->_black_victory() or this->_draw());
}
bool Natrix::_white_move() const {
    return (this->_position._move_ctr - std::floor(this->_position._move_ctr) < 1e-7);
}
bool Natrix::_black_move() {
    return !Natrix::_white_move();
}