#include "button.h"
#include "logging.h"

//
//_____________________________________________________________________________________________________________________________


Button::Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, SDL_Renderer *renderer) {
    this->unique_id = id;
    this->x = posX;
    this->y = posY;
    this->w = width;
    this->h = height;
    this->text = text;
    this->textColour = textColour;
}

//
//_____________________________________________________________________________________________________________________________


Button::Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, string filePath, SDL_Renderer *renderer) {
    this->unique_id = id;
    this->x = posX;
    this->y = posY;
    this->w = width;
    this->h = height;
    this->text = text;
    this->textColour = textColour;
    
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        throw std::runtime_error("Failed to create texture");
    }
}

//
//_____________________________________________________________________________________________________________________________


Button::~Button() {
    SDL_DestroyTexture(texture);
}

//
//_____________________________________________________________________________________________________________________________


void Button::render(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Rect rect = {x, y, w, h};
    if (texture == nullptr) {
        SDL_SetRenderDrawColor(renderer, buttonColour.r, buttonColour.g, buttonColour.b, buttonColour.a);
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    // Render text in the middle of the button
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColour);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = {x + (w - textWidth) / 2, y + (h - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

//
//_____________________________________________________________________________________________________________________________


bool Button::isClicked(int mouseX, int mouseY) {
    return mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h;
}

//
//_____________________________________________________________________________________________________________________________
