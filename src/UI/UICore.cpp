#include "UICore.hpp"


#define OBJECT (0)
#define NAME (1)


UICore::UICore() = default;
UICore::UICore(int sdl_init_code, int sdl_image_init_code, int sdl_mixer_init_code, int audio_frequency, Uint16 audio_format, int audio_channels, int audio_chunk_size) {
    int sdl_init_result = SDL_Init(sdl_init_code);
    int sdl_image_init_result = IMG_Init(sdl_image_init_code);
    int sdl_mixer_init_result = Mix_Init(sdl_mixer_init_code);
    int sdl_ttf_init_result = TTF_Init();

    if (sdl_init_result != 0 or sdl_image_init_result != sdl_image_init_code or sdl_mixer_init_result != sdl_mixer_init_code or sdl_ttf_init_result != 0) {
        std::cout << ANSI::Red << SDL_GetError() << ANSI::Green << std::endl;
        this->quit(255);
    }

    Mix_OpenAudio(audio_frequency, audio_format, audio_channels, audio_chunk_size);
}
void UICore::set_window(int type, int x, int y, int width, int height, const std::string& title, const std::string& logotype_path) {
    this->_window = SDL_CreateWindow(title.c_str(), x, y, width, height, type);

    SDL_Surface *logotype = IMG_Load(logotype_path.c_str());
    if (logotype == nullptr) {
        std::cout << ANSI::Red << "Loading logotype " << logotype_path << "... Error." << ANSI::End << std::endl;
        this->quit(255);
    }
    std::cout << ANSI::Green << "Loading logotype " << logotype_path << "... OK." << ANSI::End << std::endl;
    SDL_SetWindowIcon(this->_window, logotype);
}
void UICore::set_renderer(int renderer_type, float global_scale_x, float global_scale_y) {
    this->_renderer = SDL_CreateRenderer(this->_window, -1, renderer_type);
    SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);

    this->_global_scale_x = global_scale_x;
    this->_global_scale_y = global_scale_y;
}
void UICore::load_texture(const std::string& path, const std::string& name) {
    SDL_Texture *texture = IMG_LoadTexture(this->_renderer, path.c_str());
    if (texture == nullptr) {
        std::cout << ANSI::Red << "Loading " << path << " as " << name << "... Error." << ANSI::End << std::endl;
        this->quit(255);
    }

    std::cout << ANSI::Green << "Loading " << path << " as " << name << "... OK." << ANSI::End << std::endl;

    this->_textures.emplace_back(texture, name);
}
void UICore::load_sound(const std::string& path, const std::string& name) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        std::cout << ANSI::Red << "Loading " << path << " as " << name << "... Error." << ANSI::End << std::endl;
        this->quit(255);
    }

    std::cout << ANSI::Green << "Loading " << path << " as " << name << "... OK." << ANSI::End << std::endl;

    this->_sounds.emplace_back(sound, name);
}
void UICore::load_font(const std::string& path, int size, const std::string& name) {
    TTF_Font *font = TTF_OpenFont(path.c_str(), size);
    if (font == nullptr) {
        std::cout << ANSI::Red << "Loading " << path << " as " << name << "... Error." << ANSI::End << std::endl;
        this->quit(255);
    }

    std::cout << ANSI::Green << "Loading " << path << " as " << name << "... OK." << ANSI::End << std::endl;

    this->_fonts.emplace_back(font, name);
}
void UICore::clear() {
    SDL_RenderClear(this->_renderer);
}
void UICore::draw_texture(const std::string& name, int x, int y, bool correct_pos, float scale, float alpha) {
    int texture_number = -1;
    for (int i = 0; i < this->_textures.size(); i = i + 1) if (std::get<NAME>(this->_textures[i]) == name) texture_number = i;

    SDL_SetTextureAlphaMod(std::get<OBJECT>(this->_textures[texture_number]), (int)(255 * alpha));

    this->_rect = {x, y};
    SDL_QueryTexture(std::get<OBJECT>(this->_textures[texture_number]), nullptr, nullptr, &this->_rect.w, &this->_rect.h);

    this->_rect.w = (int)((float)this->_rect.w * this->_global_scale_x);
    this->_rect.h = (int)((float)this->_rect.h * this->_global_scale_y);
    this->_rect.x = (int)((float)this->_rect.x * this->_global_scale_x);
    this->_rect.y = (int)((float)this->_rect.y * this->_global_scale_y);

    int old_rect_w = this->_rect.w;
    int old_rect_h = this->_rect.h;
    this->_rect.w = (int)((float)this->_rect.w * scale);
    this->_rect.h = (int)((float)this->_rect.h * scale);
    if (correct_pos) {
        this->_rect.x = this->_rect.x - (this->_rect.w - old_rect_w) / 2;
        this->_rect.y = this->_rect.y - (this->_rect.h - old_rect_h) / 2;
    }

    SDL_RenderCopy(this->_renderer, std::get<OBJECT>(this->_textures[texture_number]), nullptr, &this->_rect);
}
void UICore::draw_rectangle(int x, int y, int width, int height, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(this->_renderer, red, green, blue, alpha);

    this->_rect = {x, y, width, height};

    this->_rect.w = (int)((float)this->_rect.w * this->_global_scale_x);
    this->_rect.h = (int)((float)this->_rect.h * this->_global_scale_y);
    this->_rect.x = (int)((float)this->_rect.x * this->_global_scale_x);
    this->_rect.y = (int)((float)this->_rect.y * this->_global_scale_y);

    SDL_RenderFillRect(this->_renderer, &this->_rect);
}
void UICore::draw_text(const std::string& name, const std::string& text, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
    int font_number = -1;
    for (int i = 0; i < this->_fonts.size(); i = i + 1) if (std::get<NAME>(this->_fonts[i]) == name) font_number = i;

    SDL_Surface *surface = TTF_RenderUTF8_Blended(std::get<OBJECT>(this->_fonts[font_number]), text.c_str(), {red, green, blue, alpha});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(this->_renderer, surface);

    this->_rect = {x, y};

    SDL_QueryTexture(texture, nullptr, nullptr, &this->_rect.w, &this->_rect.h);

    this->_rect.w = (int)((float)this->_rect.w * this->_global_scale_x);
    this->_rect.h = (int)((float)this->_rect.h * this->_global_scale_y);
    this->_rect.x = (int)((float)this->_rect.x * this->_global_scale_x);
    this->_rect.y = (int)((float)this->_rect.y * this->_global_scale_y);

    SDL_RenderCopy(this->_renderer, texture, nullptr, &this->_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void UICore::update() {
    SDL_RenderPresent(this->_renderer);
}
void UICore::play_sound(const std::string& name, int channel, int loops) {
    int sound_number = -1;
    for (int i = 0; i < this->_sounds.size(); i = i + 1) if (std::get<NAME>(this->_sounds[i]) == name) sound_number = i;

    Mix_PlayChannel(channel, std::get<OBJECT>(this->_sounds[sound_number]), loops);
}
void UICore::remove_events() {
    SDL_PumpEvents();
    SDL_FlushEvents(0, UINT32_MAX);
}
std::tuple<int, int> UICore::get_display_resolution() {
    SDL_DisplayMode display_mode;
    SDL_GetDesktopDisplayMode(0, &display_mode);

    return std::make_tuple(display_mode.w, display_mode.h);
}
std::tuple<int, int> UICore::get_mouse_position() const {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse_x = (int)((float)mouse_x * std::pow(this->get_global_scale_x(), -1));
    mouse_y = (int)((float)mouse_y * std::pow(this->get_global_scale_y(), -1));

    return std::make_tuple(mouse_x, mouse_y);
}
float UICore::get_global_scale_x() const {
    return this->_global_scale_x;
}
float UICore::get_global_scale_y() const {
    return this->_global_scale_y;
}
void UICore::quit(int code) {
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_renderer);

    Mix_CloseAudio();

    for (auto & tuple : this->_textures) SDL_DestroyTexture(std::get<OBJECT>(tuple));
    for (auto & tuple : this->_sounds) Mix_FreeChunk(std::get<OBJECT>(tuple));
    for (auto & tuple : this->_fonts) TTF_CloseFont(std::get<OBJECT>(tuple));

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    std::exit(code);
}