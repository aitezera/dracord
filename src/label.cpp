#include "label.h"
#include "logging.h"

//
//_____________________________________________________________________________________________________________________________


Label::Label(std::string text, int x, int y, SDL_Color color) {
    this->text = text;
    this->x = x;
    this->y = y;
    this->font = loadFont(getFontPath("Roboto-Regular.ttf").c_str()); // Will automatically assume the font is in the assets/fonts directory | Need to see if universal fonts work
    this->color = color;
}

//
//_____________________________________________________________________________________________________________________________


Label::Label(std::string text, int x, int y, SDL_Color color, std::string fontPath) {
    this->text = text;
    this->x = x;
    this->y = y;
    this->font = loadFont(getFontPath(fontPath).c_str());
    this->color = color;
}

//
//_____________________________________________________________________________________________________________________________

Label::~Label() {
    TTF_CloseFont(font);
}

//
//_____________________________________________________________________________________________________________________________


TTF_Font* Label::getFont() {
    return this->font;
}

//
//_____________________________________________________________________________________________________________________________


TTF_Font* Label::loadFont(const char* file) {
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

    logger->Info("Loaded font successfully for Label");
    return font;
}

//
//_____________________________________________________________________________________________________________________________


void Label::render(SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

    if (!surface) {
        logger->Error(("Failed to render text for label: " + std::string(TTF_GetError())).c_str());
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

//
//_____________________________________________________________________________________________________________________________


std::string Label::getFontPath(const std::string& fontName) {
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

void Label::changeText(std::string newText) {
    this->text = newText;
}