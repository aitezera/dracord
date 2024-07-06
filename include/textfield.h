#pragma once

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>

using std::string;

class TextField {
    public:
        TextField(int x, int y, int w, int h, SDL_Renderer* renderer);
        void handleTyping(SDL_Event* event);
        void render(SDL_Renderer* renderer, TTF_Font* font);
        std::string getText() const;

    private:
        SDL_Rect rect;
        string text;
        SDL_Color textColour;
        bool active;
};