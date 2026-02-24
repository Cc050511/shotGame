#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

#include "game.h"

int main(int argc, char *argv[]) {
    Game Game;
    Game.init();

    Game.run();

    return 0;
}