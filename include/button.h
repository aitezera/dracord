#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <filesystem>

using std::string;
namespace fs = std::filesystem;

class Button {
    public:
        int unique_id;
        int x, y, w, h;
        string text, filePath;
        SDL_Texture *texture;
        TTF_Font* font;

        SDL_Color buttonColour = {255, 0, 0, 255};
        SDL_Color textColour = {0, 0, 0, 255};

        Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, SDL_Renderer *renderer);
        Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, string filePath, SDL_Renderer *renderer);
        
        ~Button();

        void render(SDL_Renderer *renderer);
        bool isClicked(int mouseX, int mouseY);

        TTF_Font* getFont();
        TTF_Font* loadFont(const char* file);
        std::string getFontPath(const std::string& fontName);
};