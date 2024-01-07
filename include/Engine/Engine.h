#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "Components.h"
#include "Debugging.h"
#include "Library.h"

#ifdef _WIN32
#pragma comment(lib, "libs/lua54.lib")
#endif

namespace Engine {
    constexpr int SampleRate = 11025;
    constexpr int Channels = 1;
    constexpr int BufferSize = 4096;
    constexpr Uint16 AudioFormat = AUDIO_U16SYS;

    extern SDL_Renderer *Renderer;
    
    // Game Environment
    extern std::vector<SDL_Texture*> TexturesIDs;

    extern std::vector<Mix_Music*> Musics;
    extern std::vector<Mix_Chunk*> Sounds;

    // Core
    void Init(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer);

    void End();
}

#endif