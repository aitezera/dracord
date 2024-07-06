#include "window.h"
#include "logging.h"

#include "button.h"
#include "label.h"
#include "textfield.h"

Label* label;
Button* button;
Button* button2;
TextField* textfield;

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
                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

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

    label = new Label("I love latinas", 10, 10, { 255, 255, 255, 255 });
    textfield = new TextField(10, 50, 200, 50, renderer);

    button = new Button(1, 100, 100, 100, 100, "Test", { 0, 255, 0, 255 }, renderer);
    button2 = new Button(2, 300, 300, 100, 100, "Test2", { 0, 0, 255, 255 }, "images/test.jpg", renderer);

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
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // This wont work, fix later
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_KP_ENTER) {
                logger->Info(textfield->getText().c_str());
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (button->isClicked(mouseX, mouseY))
                {
                    logger->Info("Button 1 clicked!");
                }
                if (button2->isClicked(mouseX, mouseY))
                {
                    logger->Info("Button 2 clicked!");
                }
            }
            textfield->handleTyping(&event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        button->render(renderer, label->getFont());
        button2->render(renderer, label->getFont());
        textfield->render(renderer, label->getFont());
        
        // Render text to screen
        label->render(renderer);
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

    if (label != nullptr) {
        logger->Info("Destroying Label");
        delete label;
    }

    if (textfield != nullptr) {
        logger->Info("Destroying TextField");
        delete textfield;
    }

    if (button != nullptr) {
        logger->Info("Destroying Buttons");
        delete button;
        delete button2;
    }

    //IMG_Quit();
    SDL_Quit();
    TTF_Quit();

    return 0;
}