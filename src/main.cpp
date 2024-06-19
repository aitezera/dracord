#include "window.h"
#include "logging.h"

int main()
{
    Window window;
    window.createWindow("Dracord");
    
    Log::destroyInstance();
    return 0;
}