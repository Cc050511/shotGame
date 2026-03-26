#ifndef SRC_ENEMY_H
#define SRC_ENEMY_H

#include "bullet.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

enum class EnemyState { MoveToFormation, InFormation, DiveBomb, Evade };

class Enemy {
  public:
    Enemy(float StartX, float StartY, float DstX, float DstY)
        : Rect{StartX, StartY, 40.0f, 40.0f}, TargetX(DstX), TargetY(DstY),
          Speed(150.0f), Active(true), State(EnemyState::MoveToFormation),
          MaxHp(3), Hp(3) {
        FireCooldown = 1500.0f + static_cast<float>(rand() % 2000);
    }

    void update(float DeltaTimeMs, const SDL_FRect &PlayerRect,
                const std::vector<Bullet> &PlayerBullets,
                std::vector<EnemyBullet> &EBullets) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;

        // 1. Evade Logic
        if (State != EnemyState::DiveBomb && State != EnemyState::Evade) {
            for (const auto &pb : PlayerBullets) {
                if (!pb.isActive())
                    continue;
                auto bRect = pb.getRect();
                // Check if bullet is below and coming at us
                if (bRect.y > Rect.y && bRect.y - Rect.y < 150.f) {
                    if (bRect.x + bRect.w > Rect.x - 15.f &&
                        bRect.x < Rect.x + Rect.w + 15.f) {
                        State = EnemyState::Evade;
                        EvadeTimer = 400.0f; // 400ms evade duration
                        EvadeDirX = (bRect.x > Rect.x + Rect.w / 2)
                                        ? -350.f
                                        : 350.f; // Escape
                        break;
                    }
                }
            }
        }

        if (State == EnemyState::Evade) {
            EvadeTimer -= DeltaTimeMs;
            Rect.x += EvadeDirX * DeltaTimeS;
            if (EvadeTimer <= 0) {
                State = EnemyState::InFormation;
            }
            if (Rect.x < 0)
                Rect.x = 0;
            if (Rect.x > 800.f - Rect.w)
                Rect.x = 800.f - Rect.w;
        } else if (State == EnemyState::MoveToFormation) {
            float dx = TargetX - Rect.x;
            float dy = TargetY - Rect.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < 5.0f) {
                Rect.x = TargetX;
                Rect.y = TargetY;
                State = EnemyState::InFormation;
            } else {
                Rect.x += (dx / dist) * Speed * 3.0f * DeltaTimeS;
                Rect.y += (dy / dist) * Speed * 3.0f * DeltaTimeS;
            }
        } else if (State == EnemyState::InFormation) {
            SwayTimer += DeltaTimeS;
            Rect.x =
                TargetX + std::sin(SwayTimer * 2.0f) * 40.0f; // Sway distance

            // Dive bomb periodically randomly
            if (rand() % 400 == 0) {
                State = EnemyState::DiveBomb;
                float dx = PlayerRect.x - Rect.x;
                float dy = PlayerRect.y - Rect.y;
                float dist = std::sqrt(dx * dx + dy * dy);
                DiveVx = (dx / dist) * 350.0f; // Fast dive
                DiveVy = (dy / dist) * 350.0f + 50.0f;
            }
        } else if (State == EnemyState::DiveBomb) {
            Rect.x += DiveVx * DeltaTimeS;
            Rect.y += DiveVy * DeltaTimeS;
            if (Rect.y > 600.f) {
                Rect.y = -50.f; // wrap to top
                State = EnemyState::MoveToFormation;
            }
        }

        // 2. Fire Logic
        if (State == EnemyState::InFormation) {
            FireCooldown -= DeltaTimeMs;
            if (FireCooldown <= 0) {
                FireCooldown =
                    2000.0f +
                    static_cast<float>(rand() % 3000); // 2~5s between shots
                float cx = Rect.x + Rect.w / 2;
                float cy = Rect.y + Rect.h;
                float px = PlayerRect.x + PlayerRect.w / 2;
                float py = PlayerRect.y + PlayerRect.h / 2;
                float dx = px - cx;
                float dy = py - cy;
                float dist = std::sqrt(dx * dx + dy * dy);
                float bx = (dx / dist) * 200.0f;
                float by = (dy / dist) * 200.0f;
                EBullets.emplace_back(cx - 4.f, cy, bx, by);
            }
        }
    }

    void draw(SDL_Renderer *Renderer) {
        if (Hp == 1) {
            SDL_SetRenderDrawColor(Renderer, 150, 0, 0, 255);
        } else if (Hp == 2) {
            SDL_SetRenderDrawColor(Renderer, 200, 50, 50, 255);
        } else {
            SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
        }
        SDL_RenderFillRect(Renderer, &Rect);
    }

    void takeDamage() {
        Hp--;
        if (Hp <= 0)
            Active = false;
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }
    [[nodiscard]] bool isActive() const { return Active; }
    void deactivate() { Active = false; }

  private:
    SDL_FRect Rect;
    float TargetX, TargetY;
    float Speed;
    bool Active;
    EnemyState State;

    int MaxHp, Hp;

    float SwayTimer{0.0f};
    float FireCooldown;

    float EvadeTimer{0.0f};
    float EvadeDirX{0.0f};

    float DiveVx{0.0f};
    float DiveVy{0.0f};
};

#endif // SRC_ENEMY_H
