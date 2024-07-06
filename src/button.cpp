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
    this->font = loadFont(getFontPath("Roboto-Regular.ttf").c_str());
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
    this->font = loadFont(getFontPath("Roboto-Regular.ttf").c_str());
    
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

void Button::render(SDL_Renderer *renderer) {
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

std::string Button::getFontPath(const std::string& fontName) {
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

TTF_Font* Button::getFont() {
    return this->font;
}

//
//_____________________________________________________________________________________________________________________________


TTF_Font* Button::loadFont(const char* file) {
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

    logger->Info("Loaded font successfully for Button");
    return font;
}

//
//_____________________________________________________________________________________________________________________________