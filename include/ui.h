#pragma once

#include <SDL2/SDL_ttf.h>

class UI {
    public:
        TTF_Font* load_font(const char* file);
        void render_text(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y, TTF_Font* font);
};