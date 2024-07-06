#pragma once

#include <SDL2/SDL_ttf.h>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using std::string;

class Label {
    public:
        string text;
        string fontPath;

        int x, y;
        TTF_Font* font;
        SDL_Color color;

        Label(string text, int x, int y, SDL_Color color);
        Label(string text, int x, int y, SDL_Color color, string fontPath);
        ~Label();

        void render(SDL_Renderer* renderer);
        TTF_Font* getFont();
        TTF_Font* loadFont(const char* file);
        std::string getFontPath(const std::string& fontName);
};