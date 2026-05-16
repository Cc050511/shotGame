#ifndef SRC_BULLET_H
#define SRC_BULLET_H

#include "constants.h"
#include <SDL3/SDL.h>

class Bullet {
  public:
    Bullet(float X, float Y)
        : Rect{X, Y, BULLET_WIDTH, BULLET_HEIGHT}, Speed(BULLET_SPEED),
          Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.y -= Speed * DeltaTimeS;
        if (Rect.y + Rect.h < 0) {
            Active = false;
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 255, 255, 0, 255); // 黄色子弹
        SDL_RenderFillRect(Renderer, &Rect);
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }
    [[nodiscard]] bool isActive() const { return Active; }
    void deactivate() { Active = false; }

  private:
    SDL_FRect Rect;
    float Speed;
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
        if (Rect.y > SCREEN_HEIGHT || Rect.x < -20 || Rect.x > SCREEN_WIDTH + 20) {
            Active = false;
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 255, 0, 255, 255); // 紫红色敌弹
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
