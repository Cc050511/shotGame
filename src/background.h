#ifndef SRC_BACKGROUND_H
#define SRC_BACKGROUND_H

#include "constants.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

struct Star {
    float X, Y;
    float Size;
    float Speed;
    float TwinklePhase;
    float TwinkleSpeed;
    Uint8 R, G, B;
};

class Background {
  public:
    Background(int Width, int Height)
        : ScreenWidth(Width), ScreenHeight(Height) {
        for (int I = 0; I < 120; ++I) {
            int ColorRoll = rand() % 10;
            Uint8 R = 255, G = 255, B = 255;
            if (ColorRoll >= 8) {
                R = 200;
                G = 220;
            } else if (ColorRoll >= 6) {
                R = 200;
                B = 255;
            }
            Stars.push_back({static_cast<float>(rand() % ScreenWidth),
                             static_cast<float>(rand() % ScreenHeight),
                             static_cast<float>(1 + rand() % 3),
                             static_cast<float>(40 + rand() % 160),
                             static_cast<float>(rand() % 628) / 100.0f,
                             2.0f + static_cast<float>(rand() % 300) / 100.0f,
                             R, G, B});
        }
    }

    void update(float DeltaTimeMs) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;
        for (auto &S : Stars) {
            S.Y += S.Speed * DeltaTimeS;
            S.TwinklePhase += S.TwinkleSpeed * DeltaTimeS;
            if (S.Y > ScreenHeight) {
                S.Y = -S.Size;
                S.X = static_cast<float>(rand() % ScreenWidth);
                S.TwinklePhase = 0.0f;
            }
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        for (const auto &S : Stars) {
            float Alpha = 60 + 60 * (0.5f + 0.5f * std::sin(S.TwinklePhase));
            Uint8 A = static_cast<Uint8>(
                std::clamp(Alpha, 0.0f, 255.0f));
            SDL_SetRenderDrawColor(Renderer, S.R, S.G, S.B, A);
            SDL_FRect Rect = {S.X, S.Y, S.Size, S.Size};
            SDL_RenderFillRect(Renderer, &Rect);
        }
    }

  private:
    std::vector<Star> Stars;
    int ScreenWidth, ScreenHeight;
};

#endif // SRC_BACKGROUND_H
