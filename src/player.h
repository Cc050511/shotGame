#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include <SDL3/SDL.h>
#include <algorithm> // 引入 std::clamp 所需的头文件

class Player {
  public:
    Player() : Rect{100.0f, 100.0f, 50.0f, 50.0f}, Speed(300.0f) {}

    void update(float DeltaTimeMs, const bool *Keys) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        if (Keys[SDL_SCANCODE_W]) Rect.y -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_S]) Rect.y += Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_A]) Rect.x -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_D]) Rect.x += Speed * DeltaTimeS;

        // 限制玩家移动范围 (假设窗口大小为 800x600)
        Rect.x = std::clamp(Rect.x, 0.0f, 800.0f - Rect.w);
        Rect.y = std::clamp(Rect.y, 0.0f, 600.0f - Rect.h);
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 0, 255, 127, 255); // 春绿色方块
        SDL_RenderFillRect(Renderer, &Rect);
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }

  private:
    SDL_FRect Rect;
    float Speed;
};

#endif // SRC_PLAYER_H
