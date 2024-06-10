#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace sdlInput {
    constexpr int WIDTH = 640*2;
    constexpr int HEIGHT = 480*2;
    inline bool running = true;


    // SDL Variables
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Surface* loadedSurface;
    SDL_Texture* texture;
    SDL_Rect sidebar;

    int createWindow(const char* windowName);
    int destroyWindow();
    void loopWindow();

    // Dracord related instances
    void updateServers();
    void updateMessages();
}