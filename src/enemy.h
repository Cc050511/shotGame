#ifndef SRC_ENEMY_H
#define SRC_ENEMY_H

#include <SDL3/SDL.h>

class Enemy {
  public:
    Enemy(float X, float Y, float Speed = 150.0f)
        : Rect{X, Y, 40.0f, 40.0f}, Speed(Speed), Active(true) {}

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        Rect.y += Speed * DeltaTimeS; // 向下移动
        if (Rect.y > 600.0f) {
            Active = false; // 超出屏幕下边缘
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255); // 红色敌人
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

#endif // SRC_ENEMY_H
