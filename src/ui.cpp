#include "ui.h"
#include "logging.h"

TTF_Font* UI::load_font(const char* file) {
    if (TTF_Init() == -1) {
        logger->Error(("Failed to initialize TTF: " + std::string(TTF_GetError())).c_str());
    }

    logger->Info("Initialized TTF Font Library");

    TTF_Font* font = TTF_OpenFont(file, 24);

    if (!font) {
        logger->Error(("Failed to load font: " + std::string(TTF_GetError())).c_str());
        return nullptr;
    }

    logger->Info(("Loaded font successfully from file: " + std::string(file)).c_str());
    return font;
}

void UI::render_text(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);

    if (!surface) {
        logger->Error(("Failed to render text: " + std::string(TTF_GetError())).c_str());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        logger->Error(("Failed to create texture: " + std::string(SDL_GetError())).c_str());
        return;
    }

    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect rect = { x, y, width, height };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
