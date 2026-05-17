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
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_RenderLine(R, (float)X1, (float)Y1, (float)X2, (float)Y2);
    };

    // Outer glow
    rect(3, 2, 14, 20, 0, 60, 30, 40);

    // Left wing
    rect(0, 12, 4, 3, 0, 100, 50);
    rect(1, 11, 3, 5, 0, 120, 60);
    rect(2, 10, 2, 7, 0, 110, 55);
    rect(0, 13, 5, 3, 0, 130, 65);
    rect(1, 12, 3, 5, 0, 140, 70);

    // Right wing
    rect(16, 12, 4, 3, 0, 100, 50);
    rect(16, 11, 3, 5, 0, 120, 60);
    rect(16, 10, 2, 7, 0, 110, 55);
    rect(15, 13, 5, 3, 0, 130, 65);
    rect(16, 12, 3, 5, 0, 140, 70);

    // Wing edge lines
    line(0, 12, 0, 15, 0, 200, 100, 180);
    line(19, 12, 19, 15, 0, 200, 100, 180);

    // Body - nose cone
    rect(9, 0, 2, 2, 0, 180, 90);
    rect(8, 2, 4, 2, 0, 190, 95);
    rect(7, 4, 6, 2, 0, 195, 97);
    rect(6, 6, 8, 2, 0, 200, 100);

    // Body - main hull
    rect(5, 8, 10, 10, 0, 180, 90);

    // Body - lower taper
    rect(6, 18, 8, 2, 0, 170, 85);
    rect(7, 20, 6, 2, 0, 160, 80);
    rect(8, 22, 4, 2, 0, 150, 75);

    // Body left edge highlights (3D effect)
    rect(5, 8, 2, 10, 0, 240, 120);
    rect(6, 6, 2, 2, 0, 245, 122);
    rect(7, 4, 2, 2, 0, 235, 117);
    rect(8, 2, 2, 2, 0, 225, 112);
    rect(9, 0, 1, 2, 0, 210, 105);

    // Cockpit glass
    rect(8, 7, 4, 5, 80, 220, 255);
    rect(9, 7, 2, 5, 180, 255, 255);
    rect(8, 7, 4, 1, 200, 255, 255);

    // Cockpit frame
    rect(7, 7, 1, 5, 0, 80, 40);
    rect(12, 7, 1, 5, 0, 80, 40);

    // Wing-body connection details
    rect(5, 11, 2, 4, 0, 210, 105);
    rect(13, 11, 2, 4, 0, 210, 105);

    // Engine glow
    rect(8, 22, 4, 1, 40, 180, 255);
    rect(7, 23, 6, 1, 60, 200, 255);
    rect(7, 24, 6, 1, 80, 220, 255, 180);
    rect(8, 25, 4, 1, 100, 240, 255, 120);

    // Engine core
    rect(9, 22, 2, 4, 200, 255, 255);
    rect(9, 21, 2, 1, 150, 255, 255);

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
    auto line = [&](int X1, int Y1, int X2, int Y2, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_RenderLine(R, (float)X1, (float)Y1, (float)X2, (float)Y2);
    };

    if (IsFast) {
        // Fast enemy - sleek gold dart
        rect(4, 0, 6, 3, 200, 160, 0);
        rect(2, 3, 10, 2, 220, 180, 0);
        rect(1, 5, 12, 4, 240, 200, 0);
        rect(2, 9, 10, 2, 220, 180, 0);
        rect(4, 11, 6, 3, 200, 160, 0);

        // Highlights
        rect(2, 5, 3, 4, 255, 255, 100);
        rect(4, 3, 6, 2, 255, 255, 150);

        // Core
        rect(6, 5, 2, 4, 255, 255, 200);
        rect(6, 6, 2, 2, 255, 255, 255);

        // Edge lines
        line(2, 3, 1, 5, 255, 220, 80, 180);
        line(11, 3, 12, 5, 255, 220, 80, 180);
    } else {
        // Normal enemy - crimson battle drone
        // Outer glow
        rect(4, 0, 10, 2, 100, 0, 0, 60);

        // Outer shell
        rect(7, 0, 4, 2, 150, 0, 0);
        rect(5, 2, 8, 2, 170, 0, 0);
        rect(3, 4, 12, 2, 180, 5, 5);
        rect(2, 6, 14, 6, 190, 10, 10);
        rect(3, 12, 12, 2, 180, 5, 5);
        rect(5, 14, 8, 2, 170, 0, 0);
        rect(7, 16, 4, 2, 150, 0, 0);

        // Inner body
        rect(5, 3, 8, 12, 210, 20, 20);
        rect(4, 5, 10, 8, 230, 30, 30);

        // Armor lines
        rect(3, 6, 1, 6, 100, 0, 0);
        rect(14, 6, 1, 6, 100, 0, 0);

        // Top armor detail
        rect(6, 2, 6, 1, 220, 40, 40);

        // Center core
        rect(7, 6, 4, 6, 255, 80, 30);
        rect(8, 7, 2, 4, 255, 150, 50);
        rect(8, 8, 2, 2, 255, 220, 100);
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
    auto line = [&](int X1, int Y1, int X2, int Y2, Uint8 R_, Uint8 G_,
                    Uint8 B_, Uint8 A_ = 255) {
        SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(R, R_, G_, B_, A_);
        SDL_RenderLine(R, (float)X1, (float)Y1, (float)X2, (float)Y2);
    };

    // Outer shadow
    rect(2, 2, 20, 16, 30, 30, 40, 40);

    // Outer armor frame
    rect(1, 3, 22, 14, 70, 70, 80);
    rect(2, 1, 20, 4, 80, 80, 90);
    rect(2, 15, 20, 4, 60, 60, 70);

    // Inner armor
    rect(4, 4, 16, 12, 90, 90, 100);
    rect(3, 5, 18, 10, 100, 100, 110);

    // Horizontal armor plates
    rect(2, 6, 20, 1, 120, 120, 130);
    rect(2, 10, 20, 1, 120, 120, 130);

    // Top plate detail
    rect(11, 2, 2, 2, 110, 110, 120);

    // Center core
    rect(9, 7, 6, 6, 140, 140, 150);
    rect(10, 8, 4, 4, 160, 160, 170);

    // Glowing eye
    rect(11, 9, 2, 2, 255, 60, 60);
    rect(11, 9, 2, 1, 255, 150, 150);

    // Corner rivets
    rect(3, 4, 2, 2, 130, 130, 140);
    rect(19, 4, 2, 2, 130, 130, 140);
    rect(3, 14, 2, 2, 80, 80, 90);
    rect(19, 14, 2, 2, 80, 80, 90);

    // Armor edge lines
    line(1, 3, 22, 3, 150, 150, 160, 120);
    line(1, 16, 22, 16, 50, 50, 60, 120);

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
    rect(0, 1, 6, 12, 255, 255, 100, 40);
    rect(1, 0, 4, 14, 255, 255, 150, 70);

    // Mid glow
    rect(1, 1, 4, 12, 255, 255, 200, 120);
    rect(2, 0, 2, 14, 255, 255, 255, 180);

    // Core bolt
    rect(2, 0, 2, 14, 255, 255, 255);
    rect(2, 0, 2, 8, 200, 255, 255);

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

    // Outer glow
    rect(0, 1, 6, 8, 255, 60, 255, 40);
    rect(1, 0, 4, 10, 255, 100, 255, 60);

    // Mid glow
    rect(1, 1, 4, 8, 255, 150, 255, 100);
    rect(2, 0, 2, 10, 255, 200, 255, 150);

    // Core
    rect(2, 1, 2, 8, 255, 255, 255);
    rect(2, 2, 2, 4, 255, 180, 255);

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

    // Diamond outer
    line(6, 0, 11, 6, 0, 200, 255, 180);
    line(11, 6, 6, 12, 0, 200, 255, 180);
    line(6, 12, 1, 6, 0, 200, 255, 180);
    line(1, 6, 6, 0, 0, 200, 255, 180);

    // Diamond inner
    line(6, 2, 9, 6, 100, 230, 255, 220);
    line(9, 6, 6, 10, 100, 230, 255, 220);
    line(6, 10, 3, 6, 100, 230, 255, 220);
    line(3, 6, 6, 2, 100, 230, 255, 220);

    // Center glow
    rect(5, 5, 2, 2, 200, 255, 255);

    // Corner sparkles
    rect(3, 3, 1, 1, 255, 255, 255, 150);
    rect(8, 3, 1, 1, 255, 255, 255, 150);
    rect(3, 8, 1, 1, 255, 255, 255, 150);
    rect(8, 8, 1, 1, 255, 255, 255, 150);

    S->end(R);
    return S;
}

#endif
