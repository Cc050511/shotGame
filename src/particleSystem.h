#ifndef SRC_PARTICLE_SYSTEM_H
#define SRC_PARTICLE_SYSTEM_H

#include "constants.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

struct Particle {
    float X, Y;
    float Vx, Vy;
    float Lifetime;
    float MaxLifetime;
    SDL_Color Color;
    bool Active;
};

class ParticleSystem {
  public:
    void emit(float X, float Y, SDL_Color Color, int Count = 10) {
        for (int I = 0; I < Count; ++I) {
            float Angle =
                static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
            float Speed =
                PARTICLE_SPEED_MIN +
                static_cast<float>(rand() % static_cast<int>(
                    PARTICLE_SPEED_MAX - PARTICLE_SPEED_MIN));
            float Lifetime =
                PARTICLE_LIFETIME_MIN_MS +
                static_cast<float>(rand() % static_cast<int>(
                    PARTICLE_LIFETIME_MAX_MS - PARTICLE_LIFETIME_MIN_MS));
            Particles.push_back(
                {X, Y, std::cos(Angle) * Speed, std::sin(Angle) * Speed,
                 Lifetime, Lifetime, Color, true});
        }
    }

    void update(float DeltaTimeMs) {
        for (auto &P : Particles) {
            if (!P.Active)
                continue;
            P.X += P.Vx * (DeltaTimeMs / 1000.0f);
            P.Y += P.Vy * (DeltaTimeMs / 1000.0f);
            P.Lifetime -= DeltaTimeMs;
            if (P.Lifetime <= 0) {
                P.Active = false;
            }
        }
        Particles.erase(
            std::remove_if(Particles.begin(), Particles.end(),
                           [](const Particle &P) { return !P.Active; }),
            Particles.end());
    }

    void draw(SDL_Renderer *Renderer) {
        for (const auto &P : Particles) {
            if (!P.Active)
                continue;
            float Alpha = P.Lifetime / P.MaxLifetime;
            SDL_SetRenderDrawColor(Renderer, P.Color.r, P.Color.g, P.Color.b,
                                   static_cast<Uint8>(255 * Alpha));
            SDL_FRect Rect = {P.X, P.Y, PARTICLE_SIZE, PARTICLE_SIZE};
            SDL_RenderFillRect(Renderer, &Rect);
        }
    }

    void clear() { Particles.clear(); }

  private:
    std::vector<Particle> Particles;
};

#endif // SRC_PARTICLE_SYSTEM_H
