#include "game.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <cstdlib>

void Game::init() {
    // 1. 初始化

    if (!AppWindow.isInitialized()) {
        return;
    }
    LastTime = SDL_GetTicks();
    reset();
}

void Game::reset() {
    GamePlayer = Player(); // Reset player
    Bullets.clear();
    Enemies.clear();
    Score = 0;
    GameOver = false;
    EnemySpawnTimer = 0.0f;
    FireCooldown = 0.0f;
}

void Game::processInput() {
    // 事件处理
    while (SDL_PollEvent(&Event)) {
        if (Event.type == SDL_EVENT_QUIT) {
            KeepRunning = false;
        }
    }
}

bool Game::checkCollision(const SDL_FRect &RectA, const SDL_FRect &RectB) {
    return (RectA.x < RectB.x + RectB.w && RectA.x + RectA.w > RectB.x &&
            RectA.y < RectB.y + RectB.h && RectA.y + RectA.h > RectB.y);
}

void Game::spawnEnemy() {
    float X = static_cast<float>(rand() % (800 - 40));
    float Speed = 100.0f + static_cast<float>(rand() % 150);
    Enemies.emplace_back(X, -40.0f, Speed);
}

void Game::update(float DeltaTimeMs) {
    const bool *Keys = SDL_GetKeyboardState(nullptr);

    if (GameOver) {
        if (Keys[SDL_SCANCODE_R]) {
            reset();
        }
        return;
    }

    GamePlayer.update(DeltaTimeMs, Keys); // 更新玩家

    if (FireCooldown > 0.0f) {
        FireCooldown -= DeltaTimeMs;
    }
    if (Keys[SDL_SCANCODE_SPACE] && FireCooldown <= 0.0f) {
        auto PlayerRect = GamePlayer.getRect();
        Bullets.emplace_back(PlayerRect.x + PlayerRect.w / 2.0f - 5.0f,
                             PlayerRect.y);
        FireCooldown = 200.0f; // 200ms between shots
    }

    // Update bullets
    for (auto &B : Bullets) {
        B.update(DeltaTimeMs);
    }
    Bullets.erase(std::remove_if(Bullets.begin(), Bullets.end(),
                                 [](const Bullet &B) { return !B.isActive(); }),
                  Bullets.end());

    // Enemy spawning
    EnemySpawnTimer -= DeltaTimeMs;
    if (EnemySpawnTimer <= 0.0f) {
        spawnEnemy();
        EnemySpawnTimer = 1000.0f; // Spawn an enemy every 1 second
    }

    // Update enemies
    for (auto &E : Enemies) {
        E.update(DeltaTimeMs);
    }
    Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(),
                                 [](const Enemy &E) { return !E.isActive(); }),
                  Enemies.end());

    // Collision detection: bullets hit enemies
    for (auto &B : Bullets) {
        if (!B.isActive())
            continue;
        for (auto &E : Enemies) {
            if (!E.isActive())
                continue;
            if (checkCollision(B.getRect(), E.getRect())) {
                B.deactivate();
                E.deactivate();
                Score += 10;
                break;
            }
        }
    }

    // Collision detection: enemies hit player
    auto PlayerRect = GamePlayer.getRect();
    for (auto &E : Enemies) {
        if (E.isActive() && checkCollision(E.getRect(), PlayerRect)) {
            GameOver = true;
        }
    }

    Overlay->update(DeltaTimeMs);
}

void Game::render() {
    // Render game objects
    // This function would typically use SDL_Renderer to draw the game state
    AppWindow.clearRenderer(); // 清理窗口

    if (!GameOver) {
        for (auto &B : Bullets) {
            B.draw(AppWindow.getRenderer());
        }
        for (auto &E : Enemies) {
            E.draw(AppWindow.getRenderer());
        }
        GamePlayer.draw(AppWindow.getRenderer()); // 绘制玩家
    } else {
        // Red square for Game Over
        SDL_FRect GameOverRect = {300.0f, 200.0f, 200.0f, 150.0f};
        SDL_SetRenderDrawColor(AppWindow.getRenderer(), 139, 0, 0, 255);
        SDL_RenderFillRect(AppWindow.getRenderer(), &GameOverRect);

        float OldScaleX, OldScaleY;
        SDL_GetRenderScale(AppWindow.getRenderer(), &OldScaleX, &OldScaleY);
        SDL_SetRenderScale(AppWindow.getRenderer(), 2.0f, 2.0f);
        SDL_SetRenderDrawColor(AppWindow.getRenderer(), 255, 255, 255, 255);
        SDL_RenderDebugText(AppWindow.getRenderer(), 165.0f, 120.0f,
                            "GAME OVER");
        SDL_RenderDebugText(AppWindow.getRenderer(), 140.0f, 140.0f,
                            "Press R to Restart");
        SDL_SetRenderScale(AppWindow.getRenderer(), OldScaleX, OldScaleY);
    }

    // Draw Score
    float OldScaleX, OldScaleY;
    SDL_GetRenderScale(AppWindow.getRenderer(), &OldScaleX, &OldScaleY);
    SDL_SetRenderScale(AppWindow.getRenderer(), 1.5f, 1.5f);
    SDL_SetRenderDrawColor(AppWindow.getRenderer(), 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(AppWindow.getRenderer(), 10.0f, 10.0f,
                              "Score: %d", Score);
    SDL_SetRenderScale(AppWindow.getRenderer(), OldScaleX, OldScaleY);

    Overlay->draw(AppWindow.getRenderer());
    SDL_RenderPresent(AppWindow.getRenderer());
}
