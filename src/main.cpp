#include "sdlinput.h"
#include "logging.h"

int main()
{
    sdlInput::createWindow("Dracord");
    Log::destroyInstance();
    return 0;
}