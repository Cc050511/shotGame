#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

#include "game.h"

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        Game Game;
        Game.init();

        Game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}