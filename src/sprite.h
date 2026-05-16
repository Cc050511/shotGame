#ifndef SRC_SPRITE_H
#define SRC_SPRITE_H

#include <SDL3/SDL.h>
#include <memory>

class Sprite {
  public:
    Sprite() = default;

    bool create(SDL_Renderer *Renderer, int W, int H) {
        Width = W;
        Height = H;
        Texture.reset(SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ABGR8888,
                                        SDL_TEXTUREACCESS_TARGET, W, H));
        if (!Texture)
            return false;
        SDL_SetTextureBlendMode(Texture.get(), SDL_BLENDMODE_BLEND);
        begin(Renderer);
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);
        end(Renderer);
        return true;
    }

    void begin(SDL_Renderer *Renderer) {
        SDL_SetRenderTarget(Renderer, Texture.get());
    }

    void end(SDL_Renderer *Renderer) {
        SDL_SetRenderTarget(Renderer, nullptr);
    }

    void draw(SDL_Renderer *Renderer, float X, float Y) const {
        SDL_FRect Dst = {X, Y, static_cast<float>(Width),
                         static_cast<float>(Height)};
        SDL_RenderTexture(Renderer, Texture.get(), nullptr, &Dst);
    }

    [[nodiscard]] int getWidth() const { return Width; }
    [[nodiscard]] int getHeight() const { return Height; }

  private:
    struct Deleter {
        void operator()(SDL_Texture *T) const {
            if (T)
                SDL_DestroyTexture(T);
        }
    };
    std::unique_ptr<SDL_Texture, Deleter> Texture;
    int Width = 0;
    int Height = 0;
};

#endif
