#include "button.h"


Button::Button(int id, int posX, int posY, int width, int height, string text, SDL_Color textColour, SDL_Renderer *renderer) {
    this->unique_id = id;
    this->x = posX;
    this->y = posY;
    this->w = width;
    this->h = height;
    this->text = text;
    this->textColour = textColour;

    SDL_Surface *surface = IMG_Load("images/test.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        throw std::runtime_error("Failed to create texture");
    }    
}


Button::~Button() {
    SDL_DestroyTexture(texture);
}


void Button::render(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

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


bool Button::isClicked(int mouseX, int mouseY) {
    return mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h;
}