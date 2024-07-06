#pragma once

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
#include <filesystem>

using std::string;
namespace fs = std::filesystem;

class TextField {
    public:
        TextField(int x, int y, int w, int h, int maxLength, SDL_Renderer* renderer);
        ~TextField();
        
        void handleTyping(SDL_Event* event);
        void render(SDL_Renderer* renderer);
        std::string getText() const;

        // Text Field font
        // Have it on it's own, so that the size can be altered without changing the font size of the entire program
        // Do the same for buttons
        TTF_Font* getFont();
        TTF_Font* loadFont(const char* file);
        std::string getFontPath(const std::string& fontName);

    private:
        string text;
        int maxLength;
        TTF_Font* font = nullptr;
        SDL_Rect rect;
        SDL_Color textColour;
        SDL_Color highlightColour;
        bool active;
        bool highlighted;
};