#ifndef SRC_BULLET_H
#define SRC_BULLET_H

#include <SDL3/SDL.h>

class Bullet {
  public:
    Bullet(float X, float Y)
        : Rect{X, Y, 10.0f, 20.0f}, Speed(500.0f), Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.y -= Speed * DeltaTimeS; // 向上移动
        if (Rect.y + Rect.h < 0) {
            Active = false; // 超出屏幕上边缘
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
        : Rect{X, Y, 8.0f, 16.0f}, VelX(Vx), VelY(Vy), Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.x += VelX * DeltaTimeS;
        Rect.y += VelY * DeltaTimeS; // move based on velocity vectors
        if (Rect.y > 600.0f || Rect.x < -20 || Rect.x > 820) {
            Active = false; // off screen
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
