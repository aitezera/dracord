#include "textfield.h"
#include "logging.h"

//
//_____________________________________________________________________________________________________________________________


TextField::TextField(int x, int y, int w, int h, int maxLength, SDL_Renderer* renderer) {
    this->rect = { x, y, w, h };
    this->text = " ";
    this->textColour = { 255, 255, 255, 255 };
    this->active = false;
    this->highlighted = false;
    this->maxLength = maxLength;
    this->highlightColour = { 255, 0, 0, 255 };
    this->font = loadFont(getFontPath("Roboto-Regular.ttf").c_str());
}

//
//_____________________________________________________________________________________________________________________________


void TextField::handleTyping(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        active = (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
    if (active && event->type == SDL_TEXTINPUT) {
        if (text.length() < maxLength) {
            text += event->text.text;
        }
    }
    if (active && event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
        text.pop_back();
    }
}

//
//_____________________________________________________________________________________________________________________________


void TextField::render(SDL_Renderer* renderer) {
    if (!font) {
        logger->Error("Font was not initalized");
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColour);

    // Fix the text going outside the textfield
    if (!surface) {
        logger->Error(("Failed to render text for textfield: " + std::string(TTF_GetError())).c_str());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        logger->Error(("Failed to create texture: " + std::string(SDL_GetError())).c_str());
        SDL_FreeSurface(surface);
        return;
    }

    int width, height;
    width = std::max(width, 1);

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect textRect = { rect.x + 5, rect.y + (rect.h - height) / 2, width, height };
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

//
//_____________________________________________________________________________________________________________________________


std::string TextField::getText() const {
    return text;
}

//
//_____________________________________________________________________________________________________________________________

std::string TextField::getFontPath(const std::string& fontName) {
    // Get the path to the current executable
    fs::path execPath = fs::current_path();

    // Go back one or two directories to ensure it's within "dracord"
    // This will unfortunately break if the executable is moved
    // Need to find a better way to do this
    execPath = execPath.parent_path().parent_path();

    logger->Info(("Current path: " + execPath.string()).c_str());

    // Append the path to the font file
    fs::path fontPath = execPath / "assets" / "fonts" / fontName;

    // Check if the font file exists
    // If not return an empty string
    if (!fs::exists(fontPath)) {
        logger->Error(("Font file does not exist: " + fontPath.string()).c_str());
        return "";
    }

    return fontPath.string();
}

//
//_____________________________________________________________________________________________________________________________

TTF_Font* TextField::getFont() {
    return this->font;
}

//
//_____________________________________________________________________________________________________________________________


TTF_Font* TextField::loadFont(const char* file) {
    if (TTF_Init() == -1) {
        logger->Error(("Failed to initialize TTF: " + std::string(TTF_GetError())).c_str());
        return nullptr;
    }

    logger->Info("Initialized TTF Font Library");

    TTF_Font* font = TTF_OpenFont(file, 24);

    if (!font) {
        logger->Error(("Failed to load font: " + std::string(TTF_GetError())).c_str());
        return nullptr;
    }

    logger->Info("Loaded font successfully for TextField");
    return font;
}

//
//_____________________________________________________________________________________________________________________________

TextField::~TextField() {
    TTF_CloseFont(font);
    TTF_Quit();
}