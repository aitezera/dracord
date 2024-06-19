#include "sdlinput.h"
#include "logging.h"

Log* logger = Log::getInstance(); // Create a new instance of the Log class

// Define the SDL variables
namespace sdlInput {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    SDL_Surface* loadedSurface = nullptr;
    SDL_Texture* texture = nullptr;
    bool running = true;

    // Components that will be used in the window
    SDL_Rect sidebar;
    SDL_Rect friend_sidebar;
    SDL_Rect topbar;

    SDL_Rect chat_window;
    SDL_Rect chat_input;
}

int sdlInput::createWindow(const char* windowName)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        logger->Error(("Failed to initialize SDL: " + std::string(SDL_GetError())).c_str());
        return 1;
    }

    logger->Info("SDL initialized successfully!");

    window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

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

    /*
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        {
            logger->Error(("SDL_Image could not be initialized! Error: " + std::string(IMG_GetError())).c_str());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        logger->Info("SDL_Image initialized successfully!");

        loadedSurface = IMG_Load("/media/Software/Code/C++/dracord/images/test.jpg");
        if (loadedSurface == nullptr)
        {
            logger->Error(("Unable to load image! Error: " + std::string(IMG_GetError())).c_str());
            IMG_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
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
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    */
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
        
        // Create sidebar for guilds to go within
        sidebar.x = 0;
        sidebar.y = 0;
        sidebar.w = WIDTH / 15;
        sidebar.h = HEIGHT;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &sidebar);

        // Create sidebar for friends to go within
        friend_sidebar.x = sidebar.w;
        friend_sidebar.y = 0;
        friend_sidebar.w = WIDTH / 8;
        friend_sidebar.h = HEIGHT;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &friend_sidebar);

        // Create topbar for the server
        topbar.x = (friend_sidebar.w  + sidebar.w);
        topbar.y = 0;
        topbar.w = WIDTH - (WIDTH / 15) - (WIDTH / 15);
        topbar.h = HEIGHT / 15;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &topbar);

        // Create chat window
        chat_window.x = (friend_sidebar.w + sidebar.w);
        chat_window.y = topbar.h;
        chat_window.w = WIDTH - (WIDTH / 15) - (WIDTH / 15);
        chat_window.h = HEIGHT - (HEIGHT / 15) - (HEIGHT / 15);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &chat_window);

        // Create chat input
        chat_input.x = (friend_sidebar.w + sidebar.w);
        chat_input.y = HEIGHT - (HEIGHT / 15);
        chat_input.w = WIDTH - (WIDTH / 15) - (WIDTH / 15);
        chat_input.h = HEIGHT / 15;
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &chat_input);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    logger->Info("Window loop ended");
    destroyWindow();
}

int sdlInput::destroyWindow()
{
    logger->Info("Destroying window");
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
    return 0;
}
