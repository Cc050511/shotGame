#ifndef SRC_SPRITE_FACTORY_H
#define SRC_SPRITE_FACTORY_H

#include "sprite.h"
#include <SDL3/SDL.h>
#include <memory>

inline std::unique_ptr<Sprite> createPlayerSprite(SDL_Renderer *R) {
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, 20, 26))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };
    auto line = [&](int X1, int Y1, int X2, int Y2, Uint8 R_, Uint8 G_,
                    Uint8 B_) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, 255);
        SDL_RenderLine(R, (float)X1, (float)Y1, (float)X2, (float)Y2);
    };

    // Shadow layer
    rect(2, 14, 16, 10, 0, 60, 30, 120);

    // Left wing
    rect(0, 13, 5, 5, 0, 150, 80);
    rect(0, 14, 6, 3, 0, 180, 90);

    // Right wing
    rect(15, 13, 5, 5, 0, 150, 80);
    rect(14, 14, 6, 3, 0, 180, 90);

    // Main body
    rect(6, 2, 8, 18, 0, 180, 90);
    rect(7, 0, 6, 4, 0, 200, 100);
    rect(5, 4, 10, 16, 0, 160, 80);

    // Body highlight (top)
    rect(8, 1, 4, 2, 0, 255, 130);

    // Cockpit
    rect(8, 5, 4, 6, 120, 255, 200);
    rect(9, 6, 2, 4, 180, 255, 220);

    // Wing details
    rect(2, 15, 3, 2, 0, 200, 100);
    rect(15, 15, 3, 2, 0, 200, 100);

    // Left wingtip
    line(0, 13, 0, 18, 0, 220, 110);
    // Right wingtip
    line(19, 13, 19, 18, 0, 220, 110);

    // Engine glow (blue)
    rect(7, 20, 6, 4, 60, 220, 255);
    rect(8, 20, 4, 5, 100, 255, 255);

    // Engine center
    rect(9, 21, 2, 3, 0, 180, 255);

    S->end(R);
    return S;
}

inline std::unique_ptr<Sprite> createEnemySprite(SDL_Renderer *R,
                                                 bool IsFast) {
    int Size = IsFast ? 14 : 18;
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, Size, Size))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };

    if (IsFast) {
        // Fast enemy - small dart shape
        rect(4, 0, 6, 3, 200, 180, 0);
        rect(2, 3, 10, 4, 220, 200, 0);
        rect(1, 7, 12, 4, 240, 220, 0);
        rect(3, 11, 8, 3, 200, 180, 0);
        // Highlight
        rect(5, 2, 4, 2, 255, 255, 100);
    } else {
        // Normal enemy - hexagonal shape
        rect(6, 0, 6, 3, 180, 0, 0);
        rect(3, 3, 12, 4, 200, 0, 0);
        rect(1, 7, 16, 4, 220, 20, 20);
        rect(3, 11, 12, 4, 200, 0, 0);
        rect(6, 15, 6, 3, 180, 0, 0);
        // Highlight
        rect(5, 4, 8, 3, 255, 100, 80);
    }

    S->end(R);
    return S;
}

inline std::unique_ptr<Sprite> createTankSprite(SDL_Renderer *R) {
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, 24, 20))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };

    // Outer armor
    rect(1, 2, 22, 16, 80, 80, 90);
    rect(3, 0, 18, 4, 90, 90, 100);

    // Inner body
    rect(4, 3, 16, 14, 100, 100, 110);

    // Armor lines
    rect(2, 2, 20, 2, 130, 130, 140);
    rect(2, 16, 20, 2, 60, 60, 70);

    // Center core
    rect(9, 6, 6, 8, 140, 140, 150);
    rect(10, 7, 4, 6, 160, 160, 170);

    // Eye/light
    rect(11, 8, 2, 4, 200, 200, 220);

    S->end(R);
    return S;
}

inline std::unique_ptr<Sprite> createBulletSprite(SDL_Renderer *R) {
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, 6, 14))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };

    // Outer glow
    rect(0, 1, 6, 12, 255, 255, 100, 60);
    rect(1, 0, 4, 14, 255, 255, 150, 100);

    // Core bolt
    rect(2, 0, 2, 14, 255, 255, 200);
    rect(2, 1, 2, 12, 255, 255, 255);

    S->end(R);
    return S;
}

inline std::unique_ptr<Sprite> createEnemyBulletSprite(SDL_Renderer *R) {
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, 6, 10))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };

    // Glow
    rect(0, 1, 6, 8, 255, 60, 255, 50);
    rect(1, 0, 4, 10, 255, 100, 255, 100);

    // Core
    rect(2, 1, 2, 8, 255, 180, 255);
    rect(2, 2, 2, 6, 255, 220, 255);

    S->end(R);
    return S;
}

inline std::unique_ptr<Sprite> createPowerUpSprite(SDL_Renderer *R) {
    auto S = std::make_unique<Sprite>();
    if (!S->create(R, 12, 12))
        return nullptr;
    S->begin(R);

    auto rect = [&](int X, int Y, int W, int H, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_FRect Rc = {(float)X, (float)Y, (float)W, (float)H};
        SDL_RenderFillRect(R, &Rc);
    };
    auto line = [&](int X1, int Y1, int X2, int Y2, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_RenderLine(R, (float)X1, (float)Y1, (float)X2, (float)Y2);
    };

    // Outer glow
    rect(1, 0, 10, 12, 0, 200, 255, 40);
    rect(0, 1, 12, 10, 0, 200, 255, 40);

    // Diamond shape (lines)
    line(6, 1, 11, 6, 0, 200, 255, 180);
    line(11, 6, 6, 11, 0, 200, 255, 180);
    line(6, 11, 1, 6, 0, 200, 255, 180);
    line(1, 6, 6, 1, 0, 200, 255, 180);

    // Inner diamond
    line(6, 3, 9, 6, 100, 230, 255, 220);
    line(9, 6, 6, 9, 100, 230, 255, 220);
    line(6, 9, 3, 6, 100, 230, 255, 220);
    line(3, 6, 6, 3, 100, 230, 255, 220);

    // Center glow
    rect(5, 5, 2, 2, 200, 255, 255);

    S->end(R);
    return S;
}

#endif
