#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <iostream>
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
#include "Library.h"

#ifdef _WIN32
#pragma comment(lib, "libs/lua54.lib")
#endif

namespace Engine {
    constexpr int SampleRate = 22050;
    constexpr int Channels = 1;
    constexpr int BufferSize = 4096;
    constexpr Uint16 AudioFormat = AUDIO_S16SYS;

    extern SDL_Renderer *Renderer;
    
    // Game Environment
    extern int MusicsArraySize;
    extern int SoundsArraySize;

    extern Mix_Music **Musics;
    extern Mix_Chunk **Sounds;

    // Debbuguing
    void PrintSeparator();

    // Core
    void Init(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer, int arr_mus_siz=1);

    void End();

    // Components
    class Scene {
    public:
        virtual ~Scene(){};

        bool pause;
        int mouse_x, mouse_y;
        float delta;

        virtual void Init() {}
        virtual void InputDown(SDL_Keycode) {}
        virtual void InputUp(SDL_Keycode) {}
        virtual void MouseMotion(SDL_MouseButtonEvent) {}
        virtual void MouseDown(SDL_MouseButtonEvent) {}
        virtual void MouseUp(SDL_MouseButtonEvent) {}
        virtual void Update(float delta) {}
        virtual void Render() {}
    };

    class Group;
}

#endif