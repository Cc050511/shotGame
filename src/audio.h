#ifndef SRC_AUDIO_H
#define SRC_AUDIO_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

class Audio {
  public:
    Audio() {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
            fprintf(stderr, "Audio init failed: %s\n", SDL_GetError());
            return;
        }
        Device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        if (Device == 0) {
            fprintf(stderr, "Audio device open failed: %s\n", SDL_GetError());
            return;
        }
        Ready = true;
    }

    ~Audio() {
        for (auto &S : Sounds) {
            if (S.Stream) {
                SDL_DestroyAudioStream(S.Stream);
            }
            if (S.Data) {
                SDL_free(S.Data);
            }
        }
        if (Device) {
            SDL_CloseAudioDevice(Device);
        }
    }

    bool load(const std::string &Name, const std::string &Path) {
        if (!Ready)
            return false;

        Sound S{};
        SDL_AudioSpec Spec;

        if (!SDL_LoadWAV(Path.c_str(), &Spec, &S.Data, &S.Len)) {
            fprintf(stderr, "Failed to load %s: %s\n", Path.c_str(),
                    SDL_GetError());
            return false;
        }

        S.Stream = SDL_CreateAudioStream(&Spec, nullptr);
        if (!S.Stream) {
            fprintf(stderr, "Failed to create stream for %s: %s\n",
                    Path.c_str(), SDL_GetError());
            SDL_free(S.Data);
            return false;
        }

        if (!SDL_BindAudioStream(Device, S.Stream)) {
            fprintf(stderr, "Failed to bind stream for %s: %s\n",
                    Path.c_str(), SDL_GetError());
            SDL_DestroyAudioStream(S.Stream);
            SDL_free(S.Data);
            return false;
        }

        SoundMap[Name] = Sounds.size();
        Sounds.push_back(S);
        return true;
    }

    void play(const std::string &Name) {
        if (!Ready)
            return;
        auto It = SoundMap.find(Name);
        if (It == SoundMap.end())
            return;
        auto &S = Sounds[It->second];
        SDL_PutAudioStreamData(S.Stream, S.Data, S.Len);
    }

  private:
    struct Sound {
        Uint8 *Data = nullptr;
        Uint32 Len = 0;
        SDL_AudioStream *Stream = nullptr;
    };

    bool Ready = false;
    SDL_AudioDeviceID Device = 0;
    std::vector<Sound> Sounds;
    std::unordered_map<std::string, size_t> SoundMap;
};

#endif
