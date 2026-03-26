#include "game.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <cstdlib>

void Game::init() {
    if (!AppWindow.isInitialized()) {
        return;
    }
    LastTime = SDL_GetTicks();
    reset();
}

void Game::reset() {
    GamePlayer = Player(); // Reset player
    Bullets.clear();
    EnemyBullets.clear();
    Enemies.clear();
    Score = 0;
    GameOver = false;
    EnemySpawnTimer = 0.0f;
    FireCooldown = 0.0f;
}

void Game::processInput() {
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

void Game::spawnWave() {
    int columns = 6;
    float startX = 150.0f;
    float spacingX = 100.0f;
    float startY = 40.0f;
    for (int i = 0; i < columns; i++) {
        float x = startX + i * spacingX;
        float y = startY + std::abs(2.5f - i) * 30.0f; // V shape formation
        Enemies.emplace_back(400.0f, -50.0f, x, y);
    }
}

void Game::update(float DeltaTimeMs) {
    const bool *Keys = SDL_GetKeyboardState(nullptr);

    if (GameOver) {
        if (Keys[SDL_SCANCODE_R]) {
            reset();
        }
        return;
    }

    GamePlayer.update(DeltaTimeMs, Keys);

    if (FireCooldown > 0.0f) {
        FireCooldown -= DeltaTimeMs;
    }
    if (Keys[SDL_SCANCODE_SPACE] && FireCooldown <= 0.0f) {
        auto PlayerRect = GamePlayer.getRect();
        Bullets.emplace_back(PlayerRect.x + PlayerRect.w / 2.0f - 5.0f,
                             PlayerRect.y);
        FireCooldown = 150.0f; // 150ms between shots
    }

    // Update player bullets
    for (auto &B : Bullets) {
        B.update(DeltaTimeMs);
    }
    Bullets.erase(std::remove_if(Bullets.begin(), Bullets.end(),
                                 [](const Bullet &B) { return !B.isActive(); }),
                  Bullets.end());

    // Update enemy bullets
    for (auto &EB : EnemyBullets) {
        EB.update(DeltaTimeMs);
    }
    EnemyBullets.erase(
        std::remove_if(EnemyBullets.begin(), EnemyBullets.end(),
                       [](const EnemyBullet &EB) { return !EB.isActive(); }),
        EnemyBullets.end());

    // Enemy wave spawning
    EnemySpawnTimer -= DeltaTimeMs;
    if (EnemySpawnTimer <= 0.0f) {
        spawnWave();
        EnemySpawnTimer = 6000.0f; // Spawn wave every 6 seconds
    }

    // Update enemies
    for (auto &E : Enemies) {
        E.update(DeltaTimeMs, GamePlayer.getRect(), Bullets, EnemyBullets);
    }
    Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(),
                                 [](const Enemy &E) { return !E.isActive(); }),
                  Enemies.end());

    // Collisions: Player bullets vs Enemies
    for (auto &B : Bullets) {
        if (!B.isActive())
            continue;
        for (auto &E : Enemies) {
            if (!E.isActive())
                continue;
            if (checkCollision(B.getRect(), E.getRect())) {
                B.deactivate();
                E.takeDamage();
                if (!E.isActive())
                    Score += 50;
                break; // bullet dead
            }
        }
    }

    // Collisions: Enemies vs Player
    auto PlayerRect = GamePlayer.getRect();
    for (auto &E : Enemies) {
        if (E.isActive() && checkCollision(E.getRect(), PlayerRect)) {
            GameOver = true;
        }
    }

    // Collisions: Enemy bullets vs Player
    for (auto &EB : EnemyBullets) {
        if (EB.isActive() && checkCollision(EB.getRect(), PlayerRect)) {
            GameOver = true;
        }
    }

    Overlay->update(DeltaTimeMs);
}

void Game::render() {
    AppWindow.clearRenderer();

    if (!GameOver) {
        for (auto &B : Bullets) {
            B.draw(AppWindow.getRenderer());
        }
        for (auto &EB : EnemyBullets) {
            EB.draw(AppWindow.getRenderer());
        }
        for (auto &E : Enemies) {
            E.draw(AppWindow.getRenderer());
        }
        GamePlayer.draw(AppWindow.getRenderer()); // Draw player
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
