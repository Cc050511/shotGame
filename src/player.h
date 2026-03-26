#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include <SDL3/SDL.h>

class Player {
  public:
    Player() : Rect{100.0f, 100.0f, 50.0f, 50.0f}, Speed(300.0f) {}

    void update(float DeltaTimeMs, const bool *Keys) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        if (Keys[SDL_SCANCODE_W]) Rect.y -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_S]) Rect.y += Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_A]) Rect.x -= Speed * DeltaTimeS;
        if (Keys[SDL_SCANCODE_D]) Rect.x += Speed * DeltaTimeS;
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 0, 255, 127, 255);
        SDL_RenderFillRect(Renderer, &Rect);
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }

  private:
    SDL_FRect Rect;
    float Speed;
};

#endif // SRC_PLAYER_H
