#include "game.h"
#include <SDL3/SDL_timer.h>
#include <algorithm>

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
}

void Game::update(float DeltaTimeMs) {
    // Update game state based on DeltaTimeMs
    const bool *Keys = SDL_GetKeyboardState(nullptr);
    GamePlayer.update(DeltaTimeMs, Keys); // 更新玩家
    Overlay->update(DeltaTimeMs);
}

void Game::render() {
    // Render game objects
    // This function would typically use SDL_Renderer to draw the game state
    AppWindow.clearRenderer(); // 清理窗口
    GamePlayer.draw(AppWindow.getRenderer()); // 绘制玩家
    Overlay->draw(AppWindow.getRenderer());
    SDL_RenderPresent(AppWindow.getRenderer());
}
