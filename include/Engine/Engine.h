#ifndef ENGINE_H
#define ENGINE_H


#include <vector>
#include <string>
#include <stdexcept>
#include <typeinfo>
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

#include "Core.h"
#include "Components.h"
#include "Debugging.h"
#include "Library.h"

#ifdef _WIN32
#pragma comment(lib, "libs/lua54.lib")
#endif

namespace Engine {

}

#endif