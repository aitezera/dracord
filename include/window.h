#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <memory>


class Window {
    private:
        const int WIDTH = 320*2;
        const int HEIGHT = 240*2;
        bool running = true;

    public:
        ~Window() {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
        }


        // SDL Variables
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event event = { 0 };
        SDL_Surface* loadedSurface = nullptr;
        SDL_Texture* texture = nullptr;
        
        SDL_Rect sidebar;
        SDL_Rect friend_sidebar;
        SDL_Rect topbar;
        SDL_Rect chat_window;

        SDL_Color c_sidebar = { 30, 33, 36, 255 };
        SDL_Color c_friend_sidebar = { 40, 43, 48, 255 };
        SDL_Color c_mainCol = { 54, 57, 62, 255 };

        int createWindow(const char* windowName);
        int destroyWindow();
        int loopWindow();
};