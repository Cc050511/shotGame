#ifndef SRC_BACKGROUND_H
#define SRC_BACKGROUND_H

#include "constants.h"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <vector>

struct Star {
    float X, Y;
    float Size;
    float Speed;
};

class Background {
  public:
    Background(int Width, int Height)
        : ScreenWidth(Width), ScreenHeight(Height) {
        for (int I = 0; I < 100; ++I) {
            Stars.push_back({static_cast<float>(rand() % ScreenWidth),
                             static_cast<float>(rand() % ScreenHeight),
                             static_cast<float>(1 + rand() % 3),
                             static_cast<float>(50 + rand() % 150)});
        }
    }

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        for (auto &S : Stars) {
            S.Y += S.Speed * DeltaTimeS;
            if (S.Y > ScreenHeight) {
                S.Y = -S.Size;
                S.X = static_cast<float>(rand() % ScreenWidth);
            }
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 150);
        for (const auto &S : Stars) {
            SDL_FRect Rect = {S.X, S.Y, S.Size, S.Size};
            SDL_RenderFillRect(Renderer, &Rect);
        }
    }

  private:
    std::vector<Star> Stars;
    int ScreenWidth, ScreenHeight;
};

#endif // SRC_BACKGROUND_H
