#include "sdlinput.h"
#include "logging.h"


Log* logger = Log::getInstance(); // Create a new instance of the Log class

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
        logger->Error(("Failed to initialise SDL: " + std::string(SDL_GetError())).c_str());
        return 1;
    }

    logger->Info("SDL initialised successfully!");

    window = SDL_CreateWindow(
                            windowName,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIDTH, HEIGHT, (SDL_WINDOW_SHOWN)
                            );
    
    if (!window)
    { 
        logger->Error(("Failed to create window: " + std::string(SDL_GetError())).c_str());
        SDL_Quit();
        return 1;
    }

    logger->Info("Window created successfully!");

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        logger->Error(("Failed to create renderer: " + std::string(SDL_GetError())).c_str());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    logger->Info("Renderer created successfully!");

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        logger->Error(("SDL_Image could not be initialised! Error: " + std::string(IMG_GetError())).c_str());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    logger->Info("SDL_Image initialised successfully!");

    loadedSurface = IMG_Load("/media/Software/Code/C++/dracord/images/test.jpg");
    if (loadedSurface == nullptr)
    {
        logger->Error(("Unable to load image! Error: " + std::string(IMG_GetError())).c_str());
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    logger->Info("Image loaded successfully!");

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr)
    {
        logger->Error(("Unable to create texture! Error: " + std::string(SDL_GetError())).c_str());
        SDL_FreeSurface(loadedSurface);
        IMG_Quit();
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    logger->Info("Texture created successfully!");
    SDL_FreeSurface(loadedSurface);
    
    sdlInput::loopWindow();

    return 0;
}

void sdlInput::loopWindow()
{
    logger->Info("Creating loop to run window");

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

    logger->Info("Window loop ended");
    destroyWindow();
}

int sdlInput::destroyWindow()
{
    logger->Info("Destroying window");
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
