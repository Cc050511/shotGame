#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <stdexcept>

// 自定义删除器，用于 std::unique_ptr 自动释放 SDL 资源
struct SDL_Deleter {
    void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
    void operator()(SDL_Renderer *r) const { SDL_DestroyRenderer(r); }
};

class Window {
  public:
    Window(const char *title, int width, int height) {
        SDL_Window *raw_window = nullptr;
        SDL_Renderer *raw_renderer = nullptr;

        if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &raw_window,
                                         &raw_renderer)) {
            throw std::runtime_error("Failed to create Window/Renderer: " +
                                     std::string(SDL_GetError()));
        }

        window_.reset(raw_window);
        renderer_.reset(raw_renderer);
    }

    ~Window() = default;

    SDL_Window *getWindow() const { return window_.get(); }
    SDL_Renderer *getRenderer() const { return renderer_.get(); }

    bool isInitialized() const { return window_ && renderer_; }

    void renderer(SDL_FRect &rect) {
        // 渲染绘制 (使用 get() 获取原始指针传给 SDL API)
        SDL_SetRenderDrawColor(renderer_.get(), 20, 20, 20,
                               255); // 深灰色背景
        SDL_RenderClear(renderer_.get());

        SDL_SetRenderDrawColor(renderer_.get(), 0, 255, 127,
                               255); // 春绿色方块
        SDL_RenderFillRect(renderer_.get(), &rect);
    }

  private:
    std::unique_ptr<SDL_Window, SDL_Deleter> window_;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};

#endif // WINDOW_H