#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>

using std::string;

class Button {
    public:
        int unique_id;
        int x, y, w, h;
        string text;
        SDL_Texture *texture;

        SDL_Color buttonColour = {255, 0, 0, 255};
        SDL_Color textColour = {0, 0, 0, 255};

        Button(int id, int posX, int posY, int width, int height, string text, SDL_Renderer *renderer);
        Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, SDL_Renderer *renderer);

        ~Button();

        void render(SDL_Renderer *renderer, TTF_Font *font);
        bool isClicked(int mouseX, int mouseY);
};