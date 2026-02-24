#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

// 自定义删除器，用于 std::unique_ptr 自动释放 SDL 资源
struct SDL_Deleter {
  void operator()(SDL_Window* w) const {
    SDL_DestroyWindow(w);
  }
  void operator()(SDL_Renderer* r) const {
    SDL_DestroyRenderer(r);
  }
};

struct SDLContext {
  SDLContext() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error("Failed to initialize SDL: " +
                               std::string(SDL_GetError()));
    }
  }
  ~SDLContext() {
    SDL_Quit();
  }
};

class PerformanceOverlay {
 public:
  PerformanceOverlay(int history_size = 100)
      : history_size_(history_size), frame_times_(history_size, 0.0f) {}

  // 更新性能数据
  void Update(float delta_time_ms) {
    frame_times_[index_] = delta_time_ms;
    index_ = (index_ + 1) % history_size_;
  }

  // 在指定的渲染器上绘制面板
  void Draw(SDL_Renderer* renderer) {
    const int panel_w = 200;
    const int panel_h = 100;
    const int margin = 10;

    // 1. 绘制半透明背景
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_FRect bg{margin, margin, (float)panel_w, (float)panel_h};
    SDL_RenderFillRect(renderer, &bg);

    // 2. 绘制波形图 (Frame Time Graph)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    float max_time =
        *std::max_element(frame_times_.begin(), frame_times_.end());
    if (max_time < 33.3f) max_time = 33.3f;  // 至少显示到 30FPS 的基准线

    for (int i = 0; i < history_size_ - 1; ++i) {
      int curr = (index_ + i) % history_size_;
      int next = (index_ + i + 1) % history_size_;

      float x1 = margin + (float)i * (panel_w / (float)history_size_);
      float y1 = margin + panel_h - (frame_times_[curr] / max_time) * panel_h;
      float x2 = margin + (float)(i + 1) * (panel_w / (float)history_size_);
      float y2 = margin + panel_h - (frame_times_[next] / max_time) * panel_h;

      SDL_RenderLine(renderer, x1, y1, x2, y2);
    }

    // 3. 绘制 16.6ms 基准线 (60 FPS)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    float target_y = margin + panel_h - (16.6f / max_time) * panel_h;
    SDL_RenderLine(renderer, margin, target_y, margin + panel_w, target_y);
  }

 private:
  int history_size_;
  std::vector<float> frame_times_;
  int index_ = 0;
};

class Window {
 public:
  Window(const char* title, int width, int height) {
    SDL_Window* raw_window = nullptr;
    SDL_Renderer* raw_renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &raw_window,
                                     &raw_renderer)) {
      throw std::runtime_error("Failed to create Window/Renderer: " +
                               std::string(SDL_GetError()));
    }

    window_.reset(raw_window);
    renderer_.reset(raw_renderer);
  }

  ~Window() = default;

  SDL_Window* getWindow() const {
    return window_.get();
  }
  SDL_Renderer* getRenderer() const {
    return renderer_.get();
  }

  bool isInitialized() const {
    return window_ && renderer_;
  }

  void renderer(SDL_FRect& rect) {
    // 渲染绘制 (使用 get() 获取原始指针传给 SDL API)
    SDL_SetRenderDrawColor(renderer_.get(), 20, 20, 20,
                           255);  // 深灰色背景
    SDL_RenderClear(renderer_.get());

    SDL_SetRenderDrawColor(renderer_.get(), 0, 255, 127,
                           255);  // 春绿色方块
    SDL_RenderFillRect(renderer_.get(), &rect);
  }

 private:
  std::unique_ptr<SDL_Window, SDL_Deleter> window_;
  std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};

int main(int argc, char* argv[]) {
  // 1. 初始化
  SDLContext sdl_context;
  Window app_window("SDL3 C++ Case", 800, 600);
  auto overlay = std::make_unique<PerformanceOverlay>();

  if (!app_window.isInitialized()) {
    return -1;
  }

  // 状态变量
  SDL_FRect rect{100.0f, 100.0f, 50.0f, 50.0f};
  float speedX = 5.0f;
  float speedY = 5.0f;
  bool keep_running = true;
  SDL_Event event;
  uint64_t last_time = SDL_GetTicks();

  // 3. 主循环
  while (keep_running) {
    uint64_t current_time = SDL_GetTicks();
    float dt = (current_time - last_time);  // 毫秒
    last_time = current_time;

    // 事件处理
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        keep_running = false;
      }
    }

    // 手动操作检测
    const bool* keys = SDL_GetKeyboardState(NULL);
    bool is_manual = false;

    if (keys[SDL_SCANCODE_UP]) {
      rect.y -= speedY;
      is_manual = true;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
      rect.y += speedY;
      is_manual = true;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
      rect.x -= speedX;
      is_manual = true;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
      rect.x += speedX;
      is_manual = true;
    }

    // 逻辑更新：仅在非手动模式下自动走
    if (!is_manual) {
      rect.x += speedX;
      if (rect.x > 750.0f || rect.x < 0.0f) speedX = -speedX;
      rect.y += speedY;
      if (rect.y > 550.0f || rect.y < 0.0f) speedY = -speedY;
    }
    if (rect.y < 0.0f) {
      rect.y = 0.0f;  // 重置位置
    }
    if (rect.y > 550.0f) {
      rect.y = 550.0f;  // 重置位置
    }
    // Clamp rect.x for manual movement as well
    if (rect.x < 0.0f) {
      rect.x = 0.0f;
    }
    if (rect.x > 750.0f) {
      rect.x = 750.0f;
    }

    app_window.renderer(rect);
    overlay->Update(dt);
    overlay->Draw(app_window.getRenderer());

    SDL_RenderPresent(app_window.getRenderer());

    SDL_Delay(16);  // 约 60 FPS
  }

  return 0;
}