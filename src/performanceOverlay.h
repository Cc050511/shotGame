#ifndef PERFORMANCE_OVERLAY_H
#define PERFORMANCE_OVERLAY_H

#include <SDL3/SDL_render.h>
#include <algorithm>
#include <vector>
class PerformanceOverlay {
  public:
    PerformanceOverlay(int HistorySize = 100)
        : HistorySize(HistorySize), FrameTimes(HistorySize, 0.0f) {}

    // 更新性能数据
    void update(float DeltaTimeMs) {
        FrameTimes[Index] = DeltaTimeMs;
        Index = (Index + 1) % HistorySize;
    }

    // 在指定的渲染器上绘制面板
    void draw(SDL_Renderer *Renderer) {
        const int PanelW = 200;
        const int PanelH = 100;
        const int Margin = 10;

        // 1. 绘制半透明背景
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 180);
        SDL_FRect Bg{Margin, Margin, (float)PanelW, (float)PanelH};
        SDL_RenderFillRect(Renderer, &Bg);

        // 2. 绘制波形图 (Frame Time Graph)
        SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
        float MaxTime =
            *std::ranges::max_element(FrameTimes);
        MaxTime = std::max(MaxTime, 33.3f); // 至少显示到 30FPS 的基准线

        for (int I = 0; I < HistorySize - 1; ++I) {
            int Curr = (Index + I) % HistorySize;
            int Next = (Index + I + 1) % HistorySize;

            float X1 = Margin + ((float)I * (PanelW / (float)HistorySize));
            float Y1 =
                Margin + PanelH - ((FrameTimes[Curr] / MaxTime) * PanelH);
            float X2 =
                Margin + ((float)(I + 1) * (PanelW / (float)HistorySize));
            float Y2 =
                Margin + PanelH - ((FrameTimes[Next] / MaxTime) * PanelH);

            SDL_RenderLine(Renderer, X1, Y1, X2, Y2);
        }

        // 3. 绘制 16.6ms 基准线 (60 FPS)
        SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 100);
        float TargetY = Margin + PanelH - ((16.6f / MaxTime) * PanelH);
        SDL_RenderLine(Renderer, Margin, TargetY, Margin + PanelW, TargetY);
    }

  private:
    int HistorySize;
    std::vector<float> FrameTimes;
    int Index = 0;
};

#endif // PERFORMANCE_OVERLAY_H