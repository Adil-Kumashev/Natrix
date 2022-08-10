#include <iostream>
#include <vector>
#include <tuple>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "../ANSI.hpp"


#pragma once


class UICore {
public:
    UICore();
    UICore(int sdl_init_code, int sdl_image_init_code, int sdl_mixer_init_code, int audio_frequency, Uint16 audio_format, int audio_channels, int audio_chunk_size);

    void set_window(int type, int x, int y, int width, int height, const std::string& title, const std::string& logotype_path);
    void set_renderer(int renderer_type, float global_scale_x, float global_scale_y);

    void load_texture(const std::string& path, const std::string& name);
    void load_sound(const std::string& path, const std::string& name);
    void load_font(const std::string& path, int size, const std::string& name);

    void clear();
    void draw_texture(const std::string& name, int x, int y, bool correct_pos, float scale, float alpha);
    void draw_rectangle(int x, int y, int width, int height, int red, int green, int blue, int alpha);
    void draw_text(const std::string& name, const std::string& text, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
    void update();

    void play_sound(const std::string& name, int channel, int loops);

    static void remove_events();

    static std::tuple<int, int> get_display_resolution();
    [[nodiscard]] std::tuple<int, int> get_mouse_position() const;

    [[nodiscard]] float get_global_scale_x() const;
    [[nodiscard]] float get_global_scale_y() const;

    void quit(int code);
private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;

    SDL_Rect _rect{};

    std::vector<std::tuple<SDL_Texture*, std::string>> _textures;
    std::vector<std::tuple<Mix_Chunk*, std::string>> _sounds;
    std::vector<std::tuple<TTF_Font*, std::string>> _fonts;

    float _global_scale_x;
    float _global_scale_y;
};