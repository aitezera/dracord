#include "sdlinput.h"

// Define the SDL variables
namespace sdlInput {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Surface* loadedSurface;
    SDL_Texture* texture;
    SDL_Rect sidebar;
    bool running = true;
}

int sdlInput::createWindow(const char* windowName)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow(
                            windowName,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIDTH, HEIGHT, (SDL_WINDOW_SHOWN)
                            );
    
    if (!window)
    { 
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "SDL_Image could not be initialised! Error: " << IMG_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    loadedSurface = IMG_Load("/media/Software/Code/C++/dracord/images/test.jpg");
    if (loadedSurface == nullptr)
    {
        std::cerr << "Unable to load image! Error: " << IMG_GetError() << std::endl;
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr)
    {
        std::cerr << "Unable to create texture! Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    SDL_FreeSurface(loadedSurface);
    sdlInput::loopWindow();

    return 0;
}

void sdlInput::loopWindow()
{
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

int sdlInput::destroyWindow()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void sdlInput::updateMessages() {
    std::cout << "I will do this later";
}

void sdlInput::updateServers() {
    std::cout << "I will do this later";
}
