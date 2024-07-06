#include "textfield.h"
#include "logging.h"

TextField::TextField(int x, int y, int w, int h, SDL_Renderer* renderer) {
    this->rect = { x, y, w, h };
    this->text = "";
    this->textColour = { 255, 255, 255, 255 };
    this->active = false;
}


void TextField::handleTyping(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        active = (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
    if (active && event->type == SDL_TEXTINPUT) {
        text += event->text.text;
    }
    if (active && event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
        text.pop_back();
    }
}

void TextField::render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColour);

    // There is an error with the surface ^^ Fix when I wake up
    if (!surface) {
        logger->Error(("Failed to render text for textfield: " + std::string(TTF_GetError())).c_str());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        logger->Error(("Failed to create texture: " + std::string(SDL_GetError())).c_str());
        return;
    }

    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect textRect = { rect.x + 5, rect.y + (rect.h - height) / 2, width, height };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
}

std::string TextField::getText() const {
    return text;
}