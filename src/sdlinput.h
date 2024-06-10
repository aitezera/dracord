#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace sdlInput {
    constexpr int WIDTH = 640*2;
    constexpr int HEIGHT = 480*2;
    extern bool running;

    // SDL Variables
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_Event event;
    extern SDL_Surface* loadedSurface;
    extern SDL_Texture* texture;
    extern SDL_Rect sidebar;

    int createWindow(const char* windowName);
    int destroyWindow();
    void loopWindow();

    // Dracord related instances
    void updateServers();
    void updateMessages();
}
