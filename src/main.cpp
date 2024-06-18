#include "sdlinput.h"
#include "logging.h"

int main()
{
    int result = sdlInput::createWindow("Dracord");
    Log::destroyInstance();
    return result;
}