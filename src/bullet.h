#ifndef SRC_BULLET_H
#define SRC_BULLET_H

#include "constants.h"
#include <SDL3/SDL.h>

class Bullet {
  public:
    Bullet(float X, float Y)
        : Rect{X, Y, BULLET_WIDTH, BULLET_HEIGHT}, VelX(0.0f),
          VelY(-BULLET_SPEED), Active(true) {}

    Bullet(float X, float Y, float Vx, float Vy)
        : Rect{X, Y, BULLET_WIDTH, BULLET_HEIGHT}, VelX(Vx), VelY(Vy),
          Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.x += VelX * DeltaTimeS;
        Rect.y += VelY * DeltaTimeS;
        if (Rect.y + Rect.h < 0 || Rect.y > SCREEN_HEIGHT ||
            Rect.x < -BULLET_WIDTH || Rect.x > SCREEN_WIDTH + BULLET_WIDTH) {
            Active = false;
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        SDL_FRect Glow = {Rect.x - 3.0f, Rect.y - 3.0f, Rect.w + 6.0f,
                          Rect.h + 6.0f};
        SDL_SetRenderDrawColor(Renderer, 255, 255, 100, 60);
        SDL_RenderFillRect(Renderer, &Glow);

        SDL_SetRenderDrawColor(Renderer, 255, 255, 200, 255);
        SDL_RenderFillRect(Renderer, &Rect);
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }
    [[nodiscard]] bool isActive() const { return Active; }
    void deactivate() { Active = false; }

  private:
    SDL_FRect Rect;
    float VelX, VelY;
    bool Active;
};

class EnemyBullet {
  public:
    EnemyBullet(float X, float Y, float Vx, float Vy)
        : Rect{X, Y, ENEMY_BULLET_WIDTH, ENEMY_BULLET_HEIGHT}, VelX(Vx),
          VelY(Vy), Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.x += VelX * DeltaTimeS;
        Rect.y += VelY * DeltaTimeS;
        if (Rect.y > SCREEN_HEIGHT || Rect.x < -20 ||
            Rect.x > SCREEN_WIDTH + 20) {
            Active = false;
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        SDL_FRect Glow = {Rect.x - 2.0f, Rect.y - 2.0f, Rect.w + 4.0f,
                          Rect.h + 4.0f};
        SDL_SetRenderDrawColor(Renderer, 255, 0, 255, 60);
        SDL_RenderFillRect(Renderer, &Glow);

        SDL_SetRenderDrawColor(Renderer, 255, 100, 255, 255);
        SDL_RenderFillRect(Renderer, &Rect);
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }
    [[nodiscard]] bool isActive() const { return Active; }
    void deactivate() { Active = false; }

  private:
    SDL_FRect Rect;
    float VelX, VelY;
    bool Active;
};

#endif // SRC_BULLET_H
