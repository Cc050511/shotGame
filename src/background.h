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

struct Nebula {
    float X, Y;
    float W, H;
    float Speed;
    Uint8 R, G, B;
    Uint8 Alpha;
};

class Background {
  public:
    Background(int Width, int Height)
        : ScreenWidth(Width), ScreenHeight(Height) {
        for (int I = 0; I < 200; ++I) {
            int ColorRoll = rand() % 12;
            Uint8 R = 255, G = 255, B = 255;
            if (ColorRoll >= 9) {
                R = 200; G = 220; B = 255;
            } else if (ColorRoll >= 7) {
                R = 255; G = 200; B = 220;
            } else if (ColorRoll >= 5) {
                R = 200; G = 255; B = 200;
            }
            Stars.push_back({static_cast<float>(rand() % ScreenWidth),
                             static_cast<float>(rand() % ScreenHeight),
                             static_cast<float>(1 + rand() % 3),
                             static_cast<float>(20 + rand() % 140),
                             static_cast<float>(rand() % 628) / 100.0f,
                             2.0f + static_cast<float>(rand() % 300) / 100.0f,
                             R, G, B});
        }

        for (int I = 0; I < 6; ++I) {
            int ColorRoll = rand() % 3;
            Uint8 R = 40, G = 20, B = 60;
            if (ColorRoll == 1) {
                R = 60; G = 40; B = 20;
            } else if (ColorRoll == 2) {
                R = 20; G = 40; B = 60;
            }
            Nebulas.push_back({static_cast<float>(rand() % (ScreenWidth + 200) - 100),
                               static_cast<float>(rand() % (ScreenHeight + 200) - 100),
                               static_cast<float>(100 + rand() % 200),
                               static_cast<float>(60 + rand() % 120),
                               static_cast<float>(5 + rand() % 15),
                               R, G, B,
                               static_cast<Uint8>(15 + rand() % 25)});
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
        for (auto &N : Nebulas) {
            N.Y += N.Speed * DeltaTimeS;
            if (N.Y > ScreenHeight + 100) {
                N.Y = -N.H;
                N.X = static_cast<float>(rand() % (ScreenWidth + 200) - 100);
            }
        }
    }

    void draw(SDL_Renderer *Renderer) {
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);

        for (const auto &N : Nebulas) {
            SDL_SetRenderDrawColor(Renderer, N.R, N.G, N.B, N.Alpha);
            SDL_FRect NR = {N.X, N.Y, N.W, N.H};
            SDL_RenderFillRect(Renderer, &NR);
        }

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
    std::vector<Nebula> Nebulas;
    int ScreenWidth, ScreenHeight;
};

#endif // SRC_BACKGROUND_H
