#include "window.h"
#include "logging.h"

Log* logger = Log::getInstance(); // Create a new instance of the Log class

int Window::createWindow(const char* windowName)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        logger->Error(("Failed to initialize SDL: " + std::string(SDL_GetError())).c_str());
        return 1;
    }

    logger->Info("SDL initialized successfully!");

    window = SDL_CreateWindow(windowName, 
                        SDL_WINDOWPOS_CENTERED, 
                        SDL_WINDOWPOS_CENTERED, 
                        WIDTH, HEIGHT,
                        SDL_WINDOW_SHOWN);

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
  
    logger->Info("Texture created successfully!");
    SDL_FreeSurface(loadedSurface);
    */
    loopWindow();

    return 0;
}

void Window::loopWindow()
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
        SDL_SetRenderDrawColor(renderer, c_sidebar.r, c_sidebar.g, c_sidebar.b, c_sidebar.a);
        SDL_RenderFillRect(renderer, &sidebar);

        // Create sidebar for friends to go within
        friend_sidebar.x = sidebar.w;
        friend_sidebar.y = 0;
        friend_sidebar.w = WIDTH / 8;
        friend_sidebar.h = HEIGHT;
        SDL_SetRenderDrawColor(renderer, c_friend_sidebar.r, c_friend_sidebar.g, c_friend_sidebar.b, c_friend_sidebar.a);
        SDL_RenderFillRect(renderer, &friend_sidebar);

        // Create topbar for the server
        topbar.x = (friend_sidebar.w  + sidebar.w);
        topbar.y = 0;
        topbar.w = WIDTH - (WIDTH / 15) - (WIDTH / 15);
        topbar.h = HEIGHT / 15;
        SDL_SetRenderDrawColor(renderer, c_mainCol.r, c_mainCol.g, c_mainCol.b, c_mainCol.a);
        SDL_RenderFillRect(renderer, &topbar);

        // Create chat window
        chat_window.x = (friend_sidebar.w + sidebar.w);
        chat_window.y = topbar.h;
        chat_window.w = WIDTH - (WIDTH / 15) - (WIDTH / 15);
        chat_window.h = HEIGHT - (HEIGHT / 15);
        SDL_SetRenderDrawColor(renderer, c_mainCol.r, c_mainCol.g, c_mainCol.b, c_mainCol.a);
        SDL_RenderFillRect(renderer, &chat_window);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    logger->Info("Window loop ended");
    destroyWindow();
}

int Window::destroyWindow()
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