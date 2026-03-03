#ifndef SRC_WINDOW_H
#define SRC_WINDOW_H

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <stdexcept>

// 自定义删除器，用于 std::unique_ptr 自动释放 SDL 资源
struct SdlDeleter {
    void operator()(SDL_Window *Window) const { SDL_DestroyWindow(Window); }
    void operator()(SDL_Renderer *Renderer) const {
        SDL_DestroyRenderer(Renderer);
    }
};

class Window {
  public:
    Window(const char *Title, int Width, int Height) {
        SDL_Window *RawWindow = nullptr;
        SDL_Renderer *RawRenderer = nullptr;

        if (!SDL_CreateWindowAndRenderer(Title, Width, Height, 0, &RawWindow,
                                         &RawRenderer)) {
            throw std::runtime_error("Failed to create Window/Renderer: " +
                                     std::string(SDL_GetError()));
        }

        WindowPtr.reset(RawWindow);
        RendererPtr.reset(RawRenderer);
    }

    ~Window() = default;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;

    [[nodiscard]] SDL_Window *getWindow() const { return WindowPtr.get(); }
    [[nodiscard]] SDL_Renderer *getRenderer() const {
        return RendererPtr.get();
    }

    [[nodiscard]] bool isInitialized() const {
        return WindowPtr && RendererPtr;
    }

    void renderer(SDL_FRect &Rect) {
        // 渲染绘制 (使用 get() 获取原始指针传给 SDL API)
        SDL_SetRenderDrawColor(RendererPtr.get(), 20, 20, 20,
                               255); // 深灰色背景
        SDL_RenderClear(RendererPtr.get());

        SDL_SetRenderDrawColor(RendererPtr.get(), 0, 255, 127,
                               255); // 春绿色方块
        SDL_RenderFillRect(RendererPtr.get(), &Rect);
    }

  private:
    std::unique_ptr<SDL_Window, SdlDeleter> WindowPtr;
    std::unique_ptr<SDL_Renderer, SdlDeleter> RendererPtr;
};

#endif // SRC_WINDOW_H