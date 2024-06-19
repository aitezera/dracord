#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "ui.h"

class Window : UI {
    private:
        const int WIDTH = 640*2;
        const int HEIGHT = 480*2;
        bool running;

    public:
        // SDL Variables
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        SDL_Surface* loadedSurface;
        SDL_Texture* texture;
        TTF_Font* w_font;
        
        SDL_Rect sidebar;
        SDL_Rect friend_sidebar;
        SDL_Rect topbar;
        SDL_Rect chat_window;

        SDL_Color c_sidebar = { 30, 33, 36, 255 };
        SDL_Color c_friend_sidebar = { 40, 43, 48, 255 };
        SDL_Color c_mainCol = { 54, 57, 62, 255 };

        int createWindow(const char* windowName);
        int destroyWindow();
        void loopWindow();
};