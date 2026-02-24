#ifndef GAME_H
#define GAME_H

#include "performanceOverlay.h"
#include "window.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <stdexcept>

struct SDLContext {
    SDLContext() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Failed to initialize SDL: " +
                                     std::string(SDL_GetError()));
        }
    }
    ~SDLContext() { SDL_Quit(); }
};

class Game {
  public:
    void init();

    void run() {
        // Main game loop
        while (KeepRunning) {
            uint64_t CurrentTime = SDL_GetTicks();
            float DeltaTime = (CurrentTime - LastTime); // 毫秒
            LastTime = CurrentTime;
            processInput();
            update(DeltaTime); // Pass actual DeltaTime in milliseconds
            render();

            float SleepTime = (SDL_GetTicks() - CurrentTime) > 16.0f
                                  ? 0.0f
                                  : (16.0f - (SDL_GetTicks() - CurrentTime));
            SDL_Delay(SleepTime);
        }
    }

    void processInput();

    void update(float DeltaTimeMs);

    void render();

  private:
    SDLContext SdlContext;
    Window AppWindow{"SDL3 C++ Case", 800, 600};
    std::unique_ptr<PerformanceOverlay> Overlay =
        std::make_unique<PerformanceOverlay>();

    bool KeepRunning = true;
    SDL_Event Event;
    SDL_FRect Rect{100.0f, 100.0f, 50.0f, 50.0f};

    float SpeedX = 5.0f;
    float SpeedY = 5.0f;
    uint64_t LastTime;
};

#endif // GAME_H