#include "game.h"
#include <SDL3/SDL_timer.h>

void Game::init() {
    // 1. 初始化

    if (!AppWindow.isInitialized()) {
        return;
    }
    LastTime = SDL_GetTicks();
}

void Game::processInput() {
    // 事件处理
    while (SDL_PollEvent(&Event)) {
        if (Event.type == SDL_EVENT_QUIT) {
            KeepRunning = false;
        }
    }

    // 手动操作检测
    const bool *keys = SDL_GetKeyboardState(NULL);
    bool IsManual = false;

    if (keys[SDL_SCANCODE_UP]) {
        Rect.y -= SpeedY;
        IsManual = true;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        Rect.y += SpeedY;
        IsManual = true;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        Rect.x -= SpeedX;
        IsManual = true;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        Rect.x += SpeedX;
        IsManual = true;
    }

    // 逻辑更新：仅在非手动模式下自动走
    if (!IsManual) {
        Rect.x += SpeedX;
        if (Rect.x > 750.0f || Rect.x < 0.0f)
            SpeedX = -SpeedX;
        Rect.y += SpeedY;
        if (Rect.y > 550.0f || Rect.y < 0.0f)
            SpeedY = -SpeedY;
    }
    if (Rect.y < 0.0f) {
        Rect.y = 0.0f; // 重置位置
    }
    if (Rect.y > 550.0f) {
        Rect.y = 550.0f; // 重置位置
    }
    // Clamp Rect.x for manual movement as well
    if (Rect.x < 0.0f) {
        Rect.x = 0.0f;
    }
    if (Rect.x > 750.0f) {
        Rect.x = 750.0f;
    }
}

void Game::update(float DeltaTimeMs) {
    // Update game state based on DeltaTimeMs
    Overlay->update(DeltaTimeMs);
    SDL_Delay(16); // 约 60 FPS
}

void Game::render() {
    // Render game objects
    // This function would typically use SDL_Renderer to draw the game state
    AppWindow.renderer(Rect);
    Overlay->draw(AppWindow.getRenderer());
    SDL_RenderPresent(AppWindow.getRenderer());
}