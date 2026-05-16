#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include "constants.h"
#include <SDL3/SDL.h>
#include <algorithm>

class Player {
  public:
    Player()
        : Rect{SCREEN_WIDTH / 2.0f - PLAYER_SIZE / 2.0f,
               SCREEN_HEIGHT - PLAYER_SIZE - 50.0f, PLAYER_SIZE, PLAYER_SIZE},
          Speed(PLAYER_SPEED),
          MaxHp(PLAYER_MAX_HP), Hp(PLAYER_MAX_HP),
          InvulTimer(0.0f) {}

    void update(float DeltaTimeMs, const bool *Keys) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        if (Keys[SDL_SCANCODE_W] || Keys[SDL_SCANCODE_UP])
            Rect.y -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_S] || Keys[SDL_SCANCODE_DOWN])
            Rect.y += Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_A] || Keys[SDL_SCANCODE_LEFT])
            Rect.x -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_D] || Keys[SDL_SCANCODE_RIGHT])
            Rect.x += Speed * DeltaTimeS;

        Rect.x = std::clamp(Rect.x, 0.0f, static_cast<float>(SCREEN_WIDTH) - Rect.w);
        Rect.y = std::clamp(Rect.y, 0.0f, static_cast<float>(SCREEN_HEIGHT) - Rect.h);

        if (InvulTimer > 0.0f) {
            InvulTimer -= DeltaTimeMs;
        }
    }

    void draw(SDL_Renderer *Renderer) {
        if (InvulTimer > 0.0f && (static_cast<int>(InvulTimer / 100) % 2 == 0)) {
        } else {
            SDL_SetRenderDrawColor(Renderer, 0, 255, 127, 255);
            SDL_RenderFillRect(Renderer, &Rect);
        }

        SDL_FRect HealthBarBg = {Rect.x, Rect.y - 10.0f, Rect.w, 5.0f};
        SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(Renderer, &HealthBarBg);

        SDL_FRect HealthBar = {
            Rect.x, Rect.y - 10.0f,
            Rect.w * (static_cast<float>(Hp) / MaxHp), 5.0f};
        SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(Renderer, &HealthBar);
    }

    bool takeDamage(int Amount = 1) {
        if (InvulTimer > 0.0f)
            return false;
        Hp -= Amount;
        if (Hp < 0)
            Hp = 0;
        InvulTimer = PLAYER_INVULN_TIME_MS;
        return true;
    }

    [[nodiscard]] int getHp() const { return Hp; }
    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }

  private:
    SDL_FRect Rect;
    float Speed;
    int MaxHp, Hp;
    float InvulTimer;
};

#endif // SRC_PLAYER_H
