#include "game.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

constexpr float SpreadAngle = 0.25f;

void Game::init() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    if (!AppWindow.isInitialized()) {
        return;
    }
    LastTime = SDL_GetTicks();
    reset();
}

void Game::reset() {
    GamePlayer = Player();
    Bullets.clear();
    EnemyBullets.clear();
    Enemies.clear();
    PowerUps.clear();
    Particles.clear();
    Score = 0;
    WeaponLevel = 1;
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
    int WaveType = rand() % 3;

    for (int I = 0; I < WAVE_COLUMNS; I++) {
        float X = WAVE_START_X + I * WAVE_SPACING_X;
        float Y = WAVE_START_Y + std::abs(2.5f - I) * 30.0f;

        EnemyType Type = EnemyType::Normal;
        if (WaveType == 1 && (I == 0 || I == 5)) {
            Type = EnemyType::Fast;
        } else if (WaveType == 2 && (I == 2 || I == 3)) {
            Type = EnemyType::Tank;
        }

        Enemies.emplace_back(400.0f, -50.0f, X, Y, Type);
    }
}

void Game::update(float DeltaTimeMs) {
    const bool *Keys = SDL_GetKeyboardState(nullptr);

    Starfield.update(DeltaTimeMs);
    Particles.update(DeltaTimeMs);

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
        auto PRect = GamePlayer.getRect();
        float CenterX = PRect.x + PRect.w / 2.0f;
        float TopY = PRect.y;
        float BW2 = BULLET_WIDTH / 2.0f;

        if (WeaponLevel >= 3) {
            float AngleSin = std::sin(SpreadAngle);
            float AngleCos = std::cos(SpreadAngle);
            float Spread = 14.0f;
            Bullets.emplace_back(CenterX - BW2, TopY, 0.0f, -BULLET_SPEED);
            Bullets.emplace_back(CenterX - Spread - BW2, TopY,
                                 -BULLET_SPEED * AngleSin,
                                 -BULLET_SPEED * AngleCos);
            Bullets.emplace_back(CenterX + Spread - BW2, TopY,
                                 BULLET_SPEED * AngleSin,
                                 -BULLET_SPEED * AngleCos);
        } else if (WeaponLevel == 2) {
            float Spread = 12.0f;
            Bullets.emplace_back(CenterX - Spread - BW2, TopY);
            Bullets.emplace_back(CenterX + Spread - BW2, TopY);
        } else {
            Bullets.emplace_back(CenterX - BW2, TopY);
        }
        FireCooldown = PLAYER_FIRE_COOLDOWN_MS;
    }

    for (auto &B : Bullets) {
        B.update(DeltaTimeMs);
    }
    Bullets.erase(std::remove_if(Bullets.begin(), Bullets.end(),
                                 [](const Bullet &B) { return !B.isActive(); }),
                  Bullets.end());

    for (auto &EB : EnemyBullets) {
        EB.update(DeltaTimeMs);
    }
    EnemyBullets.erase(
        std::remove_if(EnemyBullets.begin(), EnemyBullets.end(),
                       [](const EnemyBullet &EB) { return !EB.isActive(); }),
        EnemyBullets.end());

    EnemySpawnTimer -= DeltaTimeMs;
    if (EnemySpawnTimer <= 0.0f) {
        spawnWave();
        EnemySpawnTimer = WAVE_SPAWN_INTERVAL_MS;
    }

    for (auto &E : Enemies) {
        E.update(DeltaTimeMs, GamePlayer.getRect(), Bullets, EnemyBullets);
    }
    Enemies.erase(std::remove_if(Enemies.begin(), Enemies.end(),
                                 [](const Enemy &E) { return !E.isActive(); }),
                  Enemies.end());

    for (auto &P : PowerUps) {
        if (!P.Active)
            continue;
        P.Rect.y += P.Speed * (DeltaTimeMs / 1000.0f);
        if (P.Rect.y > SCREEN_HEIGHT) {
            P.Active = false;
        }
    }
    PowerUps.erase(
        std::remove_if(PowerUps.begin(), PowerUps.end(),
                       [](const PowerUp &P) { return !P.Active; }),
        PowerUps.end());

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
                auto ERect = E.getRect();
                if (!E.isActive()) {
                    Score += 50;
                    Particles.emit(ERect.x + ERect.w / 2,
                                   ERect.y + ERect.h / 2,
                                   {255, 0, 0, 255}, 20);
                    if (WeaponLevel < 3 && rand() % 100 < 20) {
                        PowerUps.push_back(
                            {{ERect.x + ERect.w / 2 - 8.0f, ERect.y + 4.0f,
                              16.0f, 16.0f},
                             100.0f, true});
                    }
                } else {
                    Particles.emit(B.getRect().x, B.getRect().y,
                                   {255, 255, 0, 255}, 5);
                }
                break;
            }
        }
    }

    // Collisions: Enemies vs Player
    auto PlayerRect = GamePlayer.getRect();
    for (auto &E : Enemies) {
        if (E.isActive() && checkCollision(E.getRect(), PlayerRect)) {
            if (GamePlayer.takeDamage(1)) {
                Particles.emit(PlayerRect.x + PlayerRect.w / 2,
                               PlayerRect.y + PlayerRect.h / 2,
                               {0, 255, 127, 255}, 15);
                if (GamePlayer.getHp() <= 0) {
                    GameOver = true;
                }
            }
        }
    }

    // Collisions: Enemy bullets vs Player
    for (auto &EB : EnemyBullets) {
        if (EB.isActive() && checkCollision(EB.getRect(), PlayerRect)) {
            EB.deactivate();
            if (GamePlayer.takeDamage(1)) {
                Particles.emit(EB.getRect().x, EB.getRect().y,
                               {255, 0, 255, 255}, 10);
                if (GamePlayer.getHp() <= 0) {
                    GameOver = true;
                }
            }
        }
    }

    // Collisions: Power-ups vs Player
    for (auto &P : PowerUps) {
        if (P.Active && checkCollision(P.Rect, PlayerRect)) {
            WeaponLevel = std::min(WeaponLevel + 1, 3);
            P.Active = false;
            Particles.emit(P.Rect.x + 8.0f, P.Rect.y + 8.0f,
                           {0, 200, 255, 255}, 12);
        }
    }

    Overlay->update(DeltaTimeMs);
}

void Game::drawHUD() {
    auto *R = AppWindow.getRenderer();
    float OldSx, OldSy;
    SDL_GetRenderScale(R, &OldSx, &OldSy);

    SDL_SetRenderScale(R, 1.5f, 1.5f);
    SDL_SetRenderDrawColor(R, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(R, 10, 10, "SCORE: %d", Score);
    SDL_RenderDebugTextFormat(R, 10, 24, "HI: %d", HighScore);

    if (WeaponLevel >= 2) {
        SDL_RenderDebugTextFormat(R, 10, 38, "WPN Lv%d", WeaponLevel);
    }
    SDL_SetRenderScale(R, OldSx, OldSy);
}

void Game::render() {
    auto *R = AppWindow.getRenderer();
    AppWindow.clearRenderer();
    Starfield.draw(R);
    Particles.draw(R);

    if (GameOver) {
        if (Score > HighScore) {
            HighScore = Score;
        }

        SDL_FRect GameOverRect = {250.0f, 180.0f, 300.0f, 200.0f};
        SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(R, 40, 0, 0, 200);
        SDL_RenderFillRect(R, &GameOverRect);

        float OldSx, OldSy;
        SDL_GetRenderScale(R, &OldSx, &OldSy);
        SDL_SetRenderScale(R, 2.5f, 2.5f);
        SDL_SetRenderDrawColor(R, 255, 60, 60, 255);
        SDL_RenderDebugText(R, 155.0f, 78.0f, "GAME OVER");
        SDL_SetRenderScale(R, 1.5f, 1.5f);
        SDL_SetRenderDrawColor(R, 255, 255, 255, 255);
        SDL_RenderDebugTextFormat(R, 195, 105, "SCORE: %d", Score);
        SDL_RenderDebugTextFormat(R, 195, 119, "BEST:  %d", HighScore);
        SDL_SetRenderScale(R, 1.2f, 1.2f);
        SDL_SetRenderDrawColor(R, 180, 180, 180, 255);
        SDL_RenderDebugText(R, 195, 140, "Press R to Restart");
        SDL_SetRenderScale(R, OldSx, OldSy);
    } else {
        for (auto &P : PowerUps) {
            if (!P.Active)
                continue;
            SDL_SetRenderDrawColor(R, 0, 200, 255, 255);
            SDL_RenderLine(R, P.Rect.x + P.Rect.w / 2, P.Rect.y,
                           P.Rect.x + P.Rect.w, P.Rect.y + P.Rect.h / 2);
            SDL_RenderLine(R, P.Rect.x + P.Rect.w, P.Rect.y + P.Rect.h / 2,
                           P.Rect.x + P.Rect.w / 2, P.Rect.y + P.Rect.h);
            SDL_RenderLine(R, P.Rect.x + P.Rect.w / 2, P.Rect.y + P.Rect.h,
                           P.Rect.x, P.Rect.y + P.Rect.h / 2);
            SDL_RenderLine(R, P.Rect.x, P.Rect.y + P.Rect.h / 2,
                           P.Rect.x + P.Rect.w / 2, P.Rect.y);
            SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
            SDL_FRect Glow = {P.Rect.x - 2.0f, P.Rect.y - 2.0f,
                              P.Rect.w + 4.0f, P.Rect.h + 4.0f};
            SDL_SetRenderDrawColor(R, 0, 200, 255, 50);
            SDL_RenderFillRect(R, &Glow);
        }

        for (auto &B : Bullets) {
            B.draw(R);
        }
        for (auto &EB : EnemyBullets) {
            EB.draw(R);
        }
        for (auto &E : Enemies) {
            E.draw(R);
        }
        GamePlayer.draw(R);
    }

    drawHUD();
    Overlay->draw(R);
    SDL_RenderPresent(R);
}
