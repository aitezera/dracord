#include "window.h"
#include "logging.h"

Log* logger = Log::getInstance();

int SDL_main(int argc, char* argv[])
{
    Window window;
    if (window.createWindow("Dracord") != 0)
    {
        logger->Error("Failed to create window");
        logger->destroyInstance();
        return 1;
    }

    logger->destroyInstance();
    return 0;
}