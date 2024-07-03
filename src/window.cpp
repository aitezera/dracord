#include "window.h"
#include "logging.h"

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

    std::string fontPath = getFontPath("Roboto-Regular.ttf");

    if (fontPath.empty())
    {
        logger->Error("Failed to fetch font path");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    w_font = load_font(fontPath.c_str());

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

int Window::loopWindow()
{
    logger->Info("Looping Window");

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        
        // Create sidebar for guilds to go within
        SDL_SetRenderDrawColor(renderer, c_sidebar.r, c_sidebar.g, c_sidebar.b, c_sidebar.a);
        SDL_RenderFillRect(renderer, &sidebar);

        // Create sidebar for friends to go within
        SDL_SetRenderDrawColor(renderer, c_friend_sidebar.r, c_friend_sidebar.g, c_friend_sidebar.b, c_friend_sidebar.a);
        SDL_RenderFillRect(renderer, &friend_sidebar);

        // Create topbar for the server
        SDL_SetRenderDrawColor(renderer, c_mainCol.r, c_mainCol.g, c_mainCol.b, c_mainCol.a);
        SDL_RenderFillRect(renderer, &topbar);

        // Create chat window
        SDL_SetRenderDrawColor(renderer, c_mainCol.r, c_mainCol.g, c_mainCol.b, c_mainCol.a);
        SDL_RenderFillRect(renderer, &chat_window);

        // Render text to screen
        render_text(renderer, "I love latinas", { 255, 255, 255, 255 }, 10, 10, w_font);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    logger->Info("Window loop ended");
    destroyWindow();

    return 0;
}

int Window::destroyWindow()
{
    logger->Info("Destroying all components of the Window");

    //if (texture != nullptr) {
    //    logger->Info("Destroying texture");
    //    SDL_DestroyTexture(texture);
    //    texture = nullptr;
    //}

    if (renderer != nullptr) {
        logger->Info("Destroying Renderer");
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        logger->Info("Destroying Window");
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (w_font != nullptr) {
        logger->Info("Destroying TTF Font Library");
        TTF_CloseFont(w_font);
        w_font = nullptr;
    }

    //IMG_Quit();
    SDL_Quit();
    TTF_Quit();

    return 0;
}