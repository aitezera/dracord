#include "window.h"
#include "logging.h"

Log* logger = Log::getInstance();

int main()
{
    Window window;
    if (window.createWindow("Dracord") != 0)
    {
        logger->Error("Failed to create window");
        return 1;
    }

    logger->destroyInstance();
    return 0;
}