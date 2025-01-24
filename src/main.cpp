#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game.h"

int main() 
{
    Game game;
    int exitCode{ 0 };

    if (!game.Initialize()) 
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    } 
    else 
    {
        game.Run();
    }

    return exitCode;
}
