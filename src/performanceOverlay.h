#ifndef SRC_PERFORMANCE_OVERLAY_H
#define SRC_PERFORMANCE_OVERLAY_H

#include "constants.h"
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <vector>
class PerformanceOverlay {
  public:
    PerformanceOverlay(int HistorySize = PERF_HISTORY_SIZE)
        : HistorySize(HistorySize), FrameTimes(HistorySize, 0.0f) {}

    void update(float DeltaTimeMs) {
        FrameTimes[Index] = DeltaTimeMs;
        Index = (Index + 1) % HistorySize;
    }

    void draw(SDL_Renderer *Renderer) {
        const int PanelW = OVERLAY_PANEL_WIDTH;
        const int PanelH = OVERLAY_PANEL_HEIGHT;
        const int Margin = OVERLAY_MARGIN;

        float BgX = static_cast<float>(SCREEN_WIDTH - PanelW - Margin);
        float BgY = static_cast<float>(SCREEN_HEIGHT - PanelH - Margin);

        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 180);
        SDL_FRect Bg{BgX, BgY, (float)PanelW, (float)PanelH};
        SDL_RenderFillRect(Renderer, &Bg);

        SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
        float MaxTime = *std::ranges::max_element(FrameTimes);
        MaxTime = std::max(MaxTime, 33.3f);

        for (int I = 0; I < HistorySize - 1; ++I) {
            int Curr = (Index + I) % HistorySize;
            int Next = (Index + I + 1) % HistorySize;

            float X1 = BgX + ((float)I * (PanelW / (float)HistorySize));
            float Y1 =
                BgY + PanelH - ((FrameTimes[Curr] / MaxTime) * PanelH);
            float X2 =
                BgX + ((float)(I + 1) * (PanelW / (float)HistorySize));
            float Y2 =
                BgY + PanelH - ((FrameTimes[Next] / MaxTime) * PanelH);

            SDL_RenderLine(Renderer, X1, Y1, X2, Y2);
        }

        SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 100);
        float TargetY = BgY + PanelH - ((FPS_TARGET_MS / MaxTime) * PanelH);
        SDL_RenderLine(Renderer, BgX, TargetY, BgX + PanelW, TargetY);
    }

  private:
    int HistorySize;
    std::vector<float> FrameTimes;
    int Index = 0;
};

#endif // PERFORMANCE_OVERLAY_H