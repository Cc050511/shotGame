#ifndef PERFORMANCE_OVERLAY_H
#define PERFORMANCE_OVERLAY_H

#include <SDL3/SDL_render.h>
#include <algorithm>
#include <vector>
class PerformanceOverlay {
  public:
    PerformanceOverlay(int history_size = 100)
        : history_size_(history_size), frame_times_(history_size, 0.0f) {}

    // 更新性能数据
    void update(float delta_time_ms) {
        frame_times_[index_] = delta_time_ms;
        index_ = (index_ + 1) % history_size_;
    }

    // 在指定的渲染器上绘制面板
    void draw(SDL_Renderer *renderer) {
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
        if (max_time < 33.3f)
            max_time = 33.3f; // 至少显示到 30FPS 的基准线

        for (int i = 0; i < history_size_ - 1; ++i) {
            int curr = (index_ + i) % history_size_;
            int next = (index_ + i + 1) % history_size_;

            float x1 = margin + (float)i * (panel_w / (float)history_size_);
            float y1 =
                margin + panel_h - (frame_times_[curr] / max_time) * panel_h;
            float x2 =
                margin + (float)(i + 1) * (panel_w / (float)history_size_);
            float y2 =
                margin + panel_h - (frame_times_[next] / max_time) * panel_h;

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

#endif // PERFORMANCE_OVERLAY_H