#ifndef GAME_H
#define GAME_H

#include "bullet.h"
#include "enemy.h"
#include "performanceOverlay.h"
#include "player.h"
#include "window.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <stdexcept>
#include <vector>

struct SDLContext {
    SDLContext() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("Failed to initialize SDL: " +
                                     std::string(SDL_GetError()));
        }
    }
    ~SDLContext() { SDL_Quit(); }

    SDLContext(const SDLContext &) = delete;
    SDLContext &operator=(const SDLContext &) = delete;
    SDLContext(SDLContext &&) = delete;
    SDLContext &operator=(SDLContext &&) = delete;
};

class Game {
  public:
    void init();

    void run() {
        while (KeepRunning) {
            uint64_t CurrentTime = SDL_GetTicks();
            auto DeltaTime = static_cast<float>(CurrentTime - LastTime);
            LastTime = CurrentTime;
            processInput();
            update(DeltaTime);
            render();

            float SleepTime =
                static_cast<float>(SDL_GetTicks() - CurrentTime) > 16.0f
                    ? 0.0f
                    : (16.0f -
                       static_cast<float>(SDL_GetTicks() - CurrentTime));
            if (SleepTime > 0.0f) {
                SDL_Delay(static_cast<Uint32>(SleepTime));
            }
        }
    }

    void processInput();
    void update(float DeltaTimeMs);
    void render();

    void spawnWave();
    bool checkCollision(const SDL_FRect &RectA, const SDL_FRect &RectB);
    void reset();

  private:
    SDLContext SdlContext;
    Window AppWindow{"SDL3 C++ Case - Complex AI Top Down", 800, 600};
    std::unique_ptr<PerformanceOverlay> Overlay =
        std::make_unique<PerformanceOverlay>();

    bool KeepRunning = true;
    SDL_Event Event{};
    Player GamePlayer;
    uint64_t LastTime{0};

    std::vector<Bullet> Bullets;
    std::vector<EnemyBullet> EnemyBullets;
    std::vector<Enemy> Enemies;

    float EnemySpawnTimer{0.0f};
    bool GameOver{false};
    int Score{0};
    float FireCooldown{0.0f};
};

#endif // GAME_H
