#include "window.h"
#include "logging.h"

#include "button.h"
#include "label.h"
#include "textfield.h"

//
//_____________________________________________________________________________________________________________________________


Label* label;
Button* button;
Button* button2;
TextField* textfield;


//
//_____________________________________________________________________________________________________________________________


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

    logger->Info("Initalising Label");
    label = new Label("This is a label", 10, 10, { 255, 255, 255, 255 });

    logger->Info("Initalising TextField");
    textfield = new TextField(170, 350, 200, 50, 50, renderer);
    
    logger->Info("Initalising Buttons");
    button = new Button(1, 100, 100, 150, 100, "Change Text", { 0, 255, 0, 255 }, renderer);
    button2 = new Button(2, 300, 100, 150, 100, "Reverse Text", { 0, 0, 255, 255 }, "images/test.jpg", renderer);

    loopWindow();

    return 0;
}

//
//_____________________________________________________________________________________________________________________________


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
                    //logger->Info(("Text:" + textfield->getText()).c_str());
                    logger->Info(("Changing Label text to: " + textfield->getText()).c_str());
                    label->changeText(textfield->getText());
                }
                if (button2->isClicked(mouseX, mouseY))
                {
                    logger->Info("Changing label back to default: This is a label");
                    label->changeText("This is a label");
                }
            }
            textfield->handleTyping(&event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        button->render(renderer);
        button2->render(renderer);
        textfield->render(renderer);
        
        // Render text to screen
        label->render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    logger->Info("Window loop ended");
    destroyWindow();

    return 0;
}

//
//_____________________________________________________________________________________________________________________________


int Window::destroyWindow()
{
    logger->Info("Destroying Window Components");

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

    SDL_Quit();
    TTF_Quit();

    return 0;
}

//
//_____________________________________________________________________________________________________________________________
