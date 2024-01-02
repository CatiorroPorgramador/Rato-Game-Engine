#include "Engine.h"

namespace Engine {
    SDL_Renderer* Renderer = nullptr;

    Mix_Music** Musics = nullptr;
    Mix_Chunk** Sounds = nullptr;
}

void Engine::Init(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer, int arr_mus_siz, int arr_sou_siz) {
    SDL_Init(SDL_INIT_EVERYTHING);
    Engine::Renderer = sdl_renderer;

    if (arr_mus_siz > 0)
        Engine::Musics = new Mix_Music*[arr_mus_siz];
    
    if (arr_sou_siz > 0)
        Engine::Sounds = new Mix_Chunk*[arr_sou_siz];

    Mix_OpenAudio(SampleRate, AudioFormat, Channels, BufferSize);
    TTF_Init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    ImGui_ImplSDL2_InitForSDLRenderer(sdl_window, sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(sdl_renderer);
}

void Engine::End() {
    SDL_Quit();
    SDL_DestroyRenderer(Engine::Renderer);
    Mix_CloseAudio();
    TTF_Quit();

    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    int mus_len = 0;
    int sou_len = 0;

    if (Musics != nullptr && Musics[mus_len] != NULL)
        mus_len++;
    
    if (Sounds != nullptr && Sounds[mus_len] != NULL)
        sou_len++;

    if (mus_len != 0) {
        Debugging::PrintSeparator();
        for (int i{0}; i < mus_len; ++i) {
            Mix_FreeMusic(Musics[i]);
            printf("%d/%d - Free Musics...\r", i+1, mus_len);
        } putchar('\n');
        Debugging::PrintSeparator();
    } delete Musics;
    
    if (sou_len != 0) {
        for (int i{0}; i < sou_len; ++i) {
            printf("%d/%d - Free Sounds...\r", i+1, sou_len);
            Mix_FreeChunk(Sounds[i]);
        } putchar('\n');
        Debugging::PrintSeparator();
    } delete Sounds;

    printf("Engine Terminate.\n");
}

// Debugging.h
void Engine::Debugging::PrintSeparator() {
    putchar('+');
    for (int i{0}; i < 20; ++i) {
        putchar('-');
    }
    printf("+\n");
}

void Engine::Debugging::PrintLog(const char* log, Uint8 type) {
    switch (type) {
        case ENGINE_SUCCESS:
            printf("\033[32m[SUCCESS] %s.\033[0m\n", log);
        break;

    case ENGINE_ERROR:
        printf("\033[31m[ERROR] %s.\033[0m\n", log);
        break;

    case ENGINE_WARNING:
        printf("\033[33m[WARNING] %s.\033[0m\n", log);
        break;
    
    case ENGINE_LOADING:
        printf("\033[34m[LOADING] %s.\033[0m\n", log);
        break;
    
    default:
        break;
    }
}

// Library.h
int Engine::Library::__LoadDefaultLibraries(lua_State *L) {
    luaL_openlibs(L);
    printf("Lua: Default Libraries Loaded...\n");
}

int Engine::Library::__IsInputDown(lua_State *L) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    int key = static_cast<SDL_KeyCode>(luaL_checkinteger(L, 1));

    lua_pushboolean(L, state[SDL_GetScancodeFromKey(key)] != 0);
    return 1;
}

int Engine::Library::__GetCollisionTable(lua_State *L) {
    printf("Get Collide in %s\n", luaL_checkstring(L, 1));

    return 1;
}

void Engine::Library::Register(lua_State *L) {
    luaL_newlib(L, Library::Engine);
    lua_setglobal(L, "Engine");
    lua_getglobal(L, "Engine");

    // Movement Keys Register
    lua_pushinteger(L, SDLK_w);
    lua_setfield(L, -2, "KeyW");
    
    lua_pushinteger(L, SDLK_s);
    lua_setfield(L, -2, "KeyS");
    
    lua_pushinteger(L, SDLK_a);
    lua_setfield(L, -2, "KeyA");
    
    lua_pushinteger(L, SDLK_d);
    lua_setfield(L, -2, "KeyD");
    
    lua_pop(L, 1);
}

// Components.h

// LuaComponent:Class
Engine::LuaComponent::LuaComponent(const char* path) {
    this->LoadScript(path);
}

Engine::LuaComponent::LuaComponent() {
    this->Rect = SDL_Rect {0, 0, 0, 0};
}

Engine::LuaComponent::~LuaComponent() {
    lua_close(this->LuaState);
}

bool Engine::LuaComponent::LoadScript(const char *path) {
    Engine::Debugging::PrintLog(path, ENGINE_LOADING);
    
    this->LuaState = luaL_newstate();
    Library::Register(this->LuaState);
    this->LuaRow = luaL_dofile(this->LuaState, path);

    if (this->LuaRow != LUA_OK) {
        std::cout << lua_tostring(this->LuaState, -1);
        return false;
    } else {
        Engine::Debugging::PrintLog("Sucess to load", 0);
        return true;
    }
}

void Engine::LuaComponent::ScriptInit() {
    lua_getglobal(this->LuaState, "GameObject");
    if (lua_istable(this->LuaState, -1)) {
        lua_getfield(this->LuaState, -1, "Init");
        lua_pushvalue(this->LuaState, -2);
        lua_pcall(this->LuaState, 1, 0, 0);
        lua_pop(this->LuaState, 1);
    }
    lua_pop(this->LuaState, 2);
}

void Engine::LuaComponent::ScriptUpdate(float delta_time) {
        lua_getglobal(this->LuaState, "GameObject");

        if (lua_istable(this->LuaState, -1)) {
            lua_getfield(this->LuaState, -1, "Update");
            lua_pushvalue(this->LuaState, -2);
            lua_pushnumber(this->LuaState, delta_time);
            lua_pcall(this->LuaState, 2, 0, 0);

            lua_pushstring(this->LuaState, "Rect");
            lua_gettable(this->LuaState, -2);
            if (lua_istable(this->LuaState, -1)) {
                lua_rawgeti(this->LuaState, -1, 1);
                Rect.x = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);

                lua_rawgeti(this->LuaState, -1, 2);
                Rect.y = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
                
                lua_rawgeti(this->LuaState, -1, 3);
                Rect.w = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
                
                lua_rawgeti(this->LuaState, -1, 4);
                Rect.h = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
            }
        }
        lua_pop(this->LuaState, 2);
}

