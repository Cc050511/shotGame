#ifndef SRC_ENEMY_H
#define SRC_ENEMY_H

#include "bullet.h"
#include "constants.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

enum class EnemyState { MoveToFormation, InFormation, DiveBomb, Evade, Follow };
enum class EnemyType { Normal, Fast, Tank };

class Enemy {
  public:
    Enemy(float StartX, float StartY, float DstX, float DstY,
          EnemyType Type = EnemyType::Normal)
        : Rect{StartX, StartY, ENEMY_NORMAL_SIZE, ENEMY_NORMAL_SIZE},
          TargetX(DstX), TargetY(DstY), Speed(ENEMY_NORMAL_SPEED),
          Active(true), State(EnemyState::MoveToFormation), Type(Type),
          MaxHp(ENEMY_NORMAL_HP), Hp(ENEMY_NORMAL_HP) {

        switch (Type) {
        case EnemyType::Fast:
            Speed = ENEMY_FAST_SPEED;
            MaxHp = ENEMY_FAST_HP;
            Hp = ENEMY_FAST_HP;
            Rect.w = ENEMY_FAST_SIZE;
            Rect.h = ENEMY_FAST_SIZE;
            break;
        case EnemyType::Tank:
            Speed = ENEMY_TANK_SPEED;
            MaxHp = ENEMY_TANK_HP;
            Hp = ENEMY_TANK_HP;
            Rect.w = ENEMY_TANK_SIZE;
            Rect.h = ENEMY_TANK_SIZE;
            break;
        case EnemyType::Normal:
        default:
            break;
        }
        FireCooldown = 1500.0f + static_cast<float>(rand() % 2000);
    }

    void update(float DeltaTimeMs, const SDL_FRect &PlayerRect,
                const std::vector<Bullet> &PlayerBullets,
                std::vector<EnemyBullet> &EBullets) {
        float DeltaTimeS = DeltaTimeMs / 1000.0f;

        if (HitFlashTimer > 0.0f) {
            HitFlashTimer -= DeltaTimeMs;
        }

        // 1. Evade Logic
        if (State != EnemyState::DiveBomb && State != EnemyState::Evade) {
            for (const auto &PB : PlayerBullets) {
                if (!PB.isActive())
                    continue;
                auto BRect = PB.getRect();
                if (BRect.y > Rect.y &&
                    BRect.y - Rect.y < ENEMY_EVADE_RANGE) {
                    if (BRect.x + BRect.w > Rect.x - 15.f &&
                        BRect.x < Rect.x + Rect.w + 15.f) {
                        State = EnemyState::Evade;
                        EvadeTimer = ENEMY_EVADE_TIME_MS;
                        EvadeDirX = (BRect.x > Rect.x + Rect.w / 2)
                                        ? -ENEMY_EVADE_SPEED
                                        : ENEMY_EVADE_SPEED;
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
            if (Rect.x > SCREEN_WIDTH - Rect.w)
                Rect.x = SCREEN_WIDTH - Rect.w;
        } else if (State == EnemyState::MoveToFormation) {
            float Dx = TargetX - Rect.x;
            float Dy = TargetY - Rect.y;
            float Dist = std::sqrt(Dx * Dx + Dy * Dy);
            if (Dist < 5.0f) {
                Rect.x = TargetX;
                Rect.y = TargetY;
                State = EnemyState::InFormation;
            } else {
                Rect.x += (Dx / Dist) * Speed *
                          ENEMY_MOVE_TO_FORMATION_SPEED_RATIO * DeltaTimeS;
                Rect.y += (Dy / Dist) * Speed *
                          ENEMY_MOVE_TO_FORMATION_SPEED_RATIO * DeltaTimeS;
            }
        } else if (State == EnemyState::InFormation) {
            SwayTimer += DeltaTimeS;
            Rect.x = TargetX +
                     std::sin(SwayTimer * 2.0f) * ENEMY_SWAY_AMPLITUDE;

            if (rand() % 400 == 0) {
                State = EnemyState::DiveBomb;
                float Dx = PlayerRect.x - Rect.x;
                float Dy = PlayerRect.y - Rect.y;
                float Dist = std::sqrt(Dx * Dx + Dy * Dy);
                if (Dist < COLLISION_EPSILON)
                    Dist = COLLISION_EPSILON;
                DiveVx = (Dx / Dist) * ENEMY_DIVE_SPEED;
                DiveVy = (Dy / Dist) * ENEMY_DIVE_SPEED + 50.0f;
            }

            if (Type != EnemyType::Tank && rand() % 1000 == 0) {
                State = EnemyState::Follow;
            }
        } else if (State == EnemyState::DiveBomb) {
            Rect.x += DiveVx * DeltaTimeS;
            Rect.y += DiveVy * DeltaTimeS;
            if (Rect.y > SCREEN_HEIGHT) {
                Rect.y = -50.f;
                State = EnemyState::MoveToFormation;
            }
        } else if (State == EnemyState::Follow) {
            float Dx =
                PlayerRect.x + PlayerRect.w / 2 - (Rect.x + Rect.w / 2);
            float Dy = (PlayerRect.y - ENEMY_FOLLOW_OFFSET_Y) - Rect.y;
            float Dist = std::sqrt(Dx * Dx + Dy * Dy);
            if (Dist > 5.0f) {
                Rect.x += (Dx / Dist) * Speed *
                          ENEMY_FOLLOW_SPEED_RATIO * DeltaTimeS;
                Rect.y += (Dy / Dist) * Speed *
                          ENEMY_FOLLOW_SPEED_RATIO * DeltaTimeS;
            }
            if (rand() % 300 == 0)
                State = EnemyState::DiveBomb;
            if (rand() % 500 == 0)
                State = EnemyState::InFormation;
        }

        // 2. Fire Logic
        if (State == EnemyState::InFormation ||
            State == EnemyState::Follow) {
            FireCooldown -= DeltaTimeMs;
            if (FireCooldown <= 0) {
                float Interval = 2000.0f + static_cast<float>(rand() % 3000);
                if (Type == EnemyType::Tank)
                    Interval *= 0.5f;
                FireCooldown = Interval;
                float Cx = Rect.x + Rect.w / 2;
                float Cy = Rect.y + Rect.h;
                float Px = PlayerRect.x + PlayerRect.w / 2;
                float Py = PlayerRect.y + PlayerRect.h / 2;
                float Dx = Px - Cx;
                float Dy = Py - Cy;
                float Dist = std::sqrt(Dx * Dx + Dy * Dy);
                if (Dist < COLLISION_EPSILON)
                    Dist = COLLISION_EPSILON;
                float Bx = (Dx / Dist) * ENEMY_BULLET_SPEED;
                float By = (Dy / Dist) * ENEMY_BULLET_SPEED;
                EBullets.emplace_back(Cx - 4.f, Cy, Bx, By);
            }
        }
    }

    void draw(SDL_Renderer *Renderer) {
        // Body is drawn by sprite in Game::render()
        if (HitFlashTimer > 0.0f) {
            SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(Renderer, 255, 255, 255,
                                   static_cast<Uint8>(HitFlashTimer * 2.0f));
            SDL_RenderFillRect(Renderer, &Rect);
        }
    }

    void takeDamage() {
        Hp--;
        HitFlashTimer = 80.0f;
        if (Hp <= 0)
            Active = false;
    }

    [[nodiscard]] const SDL_FRect &getRect() const { return Rect; }
    [[nodiscard]] EnemyType getType() const { return Type; }
    [[nodiscard]] bool isActive() const { return Active; }
    void deactivate() { Active = false; }

  private:
    SDL_FRect Rect;
    float TargetX, TargetY;
    float Speed;
    bool Active;
    EnemyState State;
    EnemyType Type;

    int MaxHp, Hp;

    float SwayTimer{0.0f};
    float FireCooldown;
    float HitFlashTimer{0.0f};

    float EvadeTimer{0.0f};
    float EvadeDirX{0.0f};

    float DiveVx{0.0f};
    float DiveVy{0.0f};
};

#endif // SRC_ENEMY_H
