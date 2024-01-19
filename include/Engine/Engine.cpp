#include "Engine.h"

SDL_Surface *tmp_sur;

namespace Engine {
    SDL_Renderer* Renderer = nullptr;

    std::vector<SDL_Texture*> TexturesIDs;
    
    std::vector<Mix_Music*> Musics;
    std::vector<Mix_Chunk*> Sounds;

    Engine::Camera *CurrentCamera;

    std::unordered_map<std::string, Group> CurrentGroups;
    std::vector<Signal> Signals;
}

void Engine::Init(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer) {
    SDL_Init(SDL_INIT_EVERYTHING);
    Engine::Renderer = sdl_renderer;

    Mix_OpenAudio(SampleRate, AudioFormat, Channels, BufferSize);
    TTF_Init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    ImGui_ImplSDL2_InitForSDLRenderer(sdl_window, sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(sdl_renderer);
}

void Engine::End() {
    SDL_FreeSurface(tmp_sur);

    SDL_Quit();
    SDL_DestroyRenderer(Engine::Renderer);
    Mix_CloseAudio();
    TTF_Quit();

    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Debugging::PrintSeparator();
    for (int i{0}; i < TexturesIDs.size(); ++i) {
        SDL_DestroyTexture(TexturesIDs[i]);
        printf("%d/%d - Free Textures...\r", i+1, TexturesIDs.size());
    } putchar('\n');
    Debugging::PrintSeparator();

    for (int i{0}; i < Musics.size(); ++i) {
        Mix_FreeMusic(Musics[i]);
        printf("%d/%d - Free Musics...\r", i+1, Musics.size());
    } putchar('\n');
    Debugging::PrintSeparator();
    
    for (int i{0}; i < Sounds.size(); ++i) {
        printf("%d/%d - Free Sounds...\r", i+1, Sounds.size());
        Mix_FreeChunk(Sounds[i]);
    } putchar('\n');
    Debugging::PrintSeparator();

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

int Engine::Library::__HasCollisionInGroup(lua_State *L) {
    SDL_Rect tmp_rct;

    lua_getglobal(L, "GameComponent");
    if (lua_istable(L, -1)) {
        lua_pushstring(L, "Rect");
        lua_gettable(L, -2);
        if (lua_istable(L, -1)) {
            lua_rawgeti(L, -1, 1);
            tmp_rct.x = lua_tonumber(L, -1);
            lua_pop(L, 1);

            lua_rawgeti(L, -1, 2);
            tmp_rct.y = lua_tonumber(L, -1);
            lua_pop(L, 1);
            
            lua_rawgeti(L, -1, 3);
            tmp_rct.w = lua_tonumber(L, -1);
            lua_pop(L, 1);
            
            lua_rawgeti(L, -1, 4);
            tmp_rct.h = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 2);

    auto it = CurrentGroups.find(luaL_checkstring(L, 1));
    if (it != CurrentGroups.end()) {
        lua_pushboolean(L, it->second.CheckCollision(&tmp_rct));
    } else {
        Engine::Debugging::PrintLog("Non Found Group", ENGINE_ERROR);

        lua_pushboolean(L, false);
    }

    return 1;
}

int Engine::Library::__EmitSignal(lua_State *L) {
    Engine::Signal signal;
    signal.Name = luaL_checkstring(L, 1);

    switch (lua_type(L, 2)) {
        case LUA_TNIL:
            signal.SetValue<int>(-1, LUA_TNIL);
            break;
        case LUA_TBOOLEAN:
            signal.SetValue<bool>(lua_toboolean(L, 2), LUA_TBOOLEAN);
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(L, 2))
                signal.SetValue<int>(luaL_checkinteger(L, 2), ENGINE_LUA_INTEGER);
            else
                signal.SetValue<float>(luaL_checknumber(L, 2), LUA_TNUMBER);
            break;
        case LUA_TSTRING:
            signal.SetValue<std::string>(luaL_checkstring(L, 2), LUA_TSTRING);
            break;
        case LUA_TTABLE:
            
            break;
    }

    Signals.push_back(signal);

    lua_pushboolean(L, true);
    return 1;
}

int Engine::Library::__GetSignal(lua_State *L) {
    Signal sgl;

    for (Signal signal : Engine::Signals) {
        if (signal.Name == luaL_checkstring(L, 1))
            sgl = signal;
        else
            printf("%s\n", luaL_checkstring(L, 1));
    }

    if (sgl.Name != "Undefined") {
        switch (sgl.LuaType) {
        case LUA_TNIL:
            lua_pushnil(L);
            break;
        
        case LUA_TBOOLEAN:
            lua_pushboolean(L, sgl.GetValue<bool>());
            break;
        
        case ENGINE_LUA_INTEGER:
            lua_pushinteger(L, sgl.GetValue<int>());
            break;
        
        case LUA_TNUMBER:
            lua_pushnumber(L, sgl.GetValue<float>());
            break;
        
        case LUA_TSTRING:
            lua_pushstring(L, (sgl.GetValue<std::string>()).c_str());
            break;
        
        default:
            break;
        }
    }

    return 1;
}

int Engine::Library::__LoadTextureID(lua_State *L) {
    tmp_sur = IMG_Load(luaL_checkstring(L, 1));
    TexturesIDs.push_back(SDL_CreateTextureFromSurface(Engine::Renderer, tmp_sur));

    lua_pushinteger(L, TexturesIDs.size()-1);
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
    this->SrcRect = SDL_Rect {0, 0, 16, 16};
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
    lua_getglobal(this->LuaState, "GameComponent");
    if (lua_istable(this->LuaState, -1)) {
        lua_getfield(this->LuaState, -1, "Init");
        lua_pushvalue(this->LuaState, -2);
        lua_pcall(this->LuaState, 1, 0, 0);
        lua_pop(this->LuaState, 1);
    }
    lua_pop(this->LuaState, 1);
}

void Engine::LuaComponent::ScriptUpdate(float delta_time) {
        lua_getglobal(this->LuaState, "GameComponent");

        if (lua_istable(this->LuaState, -1)) {
            lua_getfield(this->LuaState, -1, "Update");
            lua_pushvalue(this->LuaState, -2);
            lua_pushnumber(this->LuaState, delta_time);
            lua_pcall(this->LuaState, 2, 0, 0);

            lua_getfield(this->LuaState, -1, "Alive");
            this->Alive = lua_toboolean(this->LuaState, -1);
            lua_pop(this->LuaState, 1);

            lua_getfield(this->LuaState, -1, "TextureID");
            this->TextureID = lua_tointeger(this->LuaState, -1);
            lua_pop(this->LuaState, 1);

            lua_getfield(this->LuaState, -1, "DirectionX");
            this->DirectionX = lua_tonumber(this->LuaState, -1);
            lua_pop(this->LuaState, 1);

            lua_getfield(this->LuaState, -1, "DirectionY");
            this->DirectionY = lua_tonumber(this->LuaState, -1);
            lua_pop(this->LuaState, 1);

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
            lua_pop(this->LuaState, 1);


            lua_pushstring(this->LuaState, "SrcRect");
            lua_gettable(this->LuaState, -2);
            if (lua_istable(this->LuaState, -1)) {
                lua_rawgeti(this->LuaState, -1, 1);
                SrcRect.x = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);

                lua_rawgeti(this->LuaState, -1, 2);
                SrcRect.y = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
                
                lua_rawgeti(this->LuaState, -1, 3);
                SrcRect.w = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
                
                lua_rawgeti(this->LuaState, -1, 4);
                SrcRect.h = lua_tonumber(this->LuaState, -1);
                lua_pop(this->LuaState, 1);
            }
        }
        lua_pop(this->LuaState, 2);
}

// Group:Class
Engine::Group::Group() {
    
}

void Engine::Group::Update(float delta_time) {
    // Remover componentes marcados como !Alive
    this->Components.erase(std::remove_if(this->Components.begin(), this->Components.end(), [](const auto &cmp) { return !cmp->Alive; }), this->Components.end());

    for (const auto &Component : this->Components) {
        Component->Update(delta_time);
    }
}

void Engine::Group::Render() {
    for (const auto &slf_cmp : this->Components) {
        if (slf_cmp != Engine::CurrentCamera->Pinned) {
            slf_cmp->Rect.x += Engine::CurrentCamera->OffSetX;
            slf_cmp->Rect.y += Engine::CurrentCamera->OffSetY;
        }

        if (slf_cmp->TextureID != -1) {
            SDL_SetRenderDrawColor(Engine::Renderer, 10, 10, 10, SDL_ALPHA_TRANSPARENT);
            SDL_RenderFillRect(Engine::Renderer, &slf_cmp->Rect);
            SDL_RenderCopy(Engine::Renderer, Engine::TexturesIDs[slf_cmp->TextureID], &slf_cmp->SrcRect, &slf_cmp->Rect);
        } else {
            SDL_SetRenderDrawColor(Engine::Renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(Engine::Renderer, &slf_cmp->Rect);
        }
    }
}

bool Engine::Group::CheckCollision(SDL_Rect *ComponentRect) {
    for (const auto &slf_cmp : this->Components) {
        if (SDL_HasIntersection(&slf_cmp->Rect, ComponentRect)) {
            return true;
        }
    }
    return false;
}

// AnimationManager:Class
Engine::AnimationManager::AnimationManager(SDL_Rect* sheet) {
    this->s = sheet;
    this->Loop = false;

    this->las_tim = SDL_GetTicks();
}

void Engine::AnimationManager::Update(float delta_time) {
    if (p) {
        Uint32 cur_tim = SDL_GetTicks();

        if ((cur_tim - this->las_tim) >= this->at*1000.f) {
            this->fr++;
            this->f = this->anim[fr];
            
            this->las_tim = cur_tim;
            this->s->x = this->f*this->jmp;
        }
        if (fr > anim.size()-1) { // Animation Finished
            this->Finished = this->Name;
            this->p = this->Loop; // if loop is true, continue animation 
            this->fr = 0;
            
            this->las_tim = cur_tim;
            this->s->x = this->f*this->jmp; 
        }
    }
}

void Engine::AnimationManager::Play(const char* name) {
    if (std::string(name) != this->Name) {
        this->p = true;
        this->anim = anims[name];
        this->Name = std::string(name);
    }
}

void Engine::AnimationManager::Stop() {
    this->Name = "";
    this->p = false;
    this->las_tim = 0;
    this->fr = 0;
    this->f = 0;
}

void Engine::AnimationManager::CreateAnimation(const char *name, std::vector<int_fast8_t> frames) {
    anims.insert(std::make_pair(name, frames));
}

void Engine::AnimationManager::SetAnimationSpeed(float seconds) {
    if (this->at != seconds) this->at = seconds;
}

// Camera:Class
Engine::Camera::Camera() {
    this->OffSetX = 0;
    this->OffSetY = 0;
}

void Engine::Camera::Update() {
    this->OffSetX += (int) this->Pinned->DirectionX;
    this->OffSetY += (int) this->Pinned->DirectionY;
}

void Engine::Camera::Linear() {
    this->OffSetX += ((this->Pinned->Rect.x) - this->OffSetX);
    this->OffSetY += ((this->Pinned->Rect.y) - this->OffSetY);
}

// Signal:Class
Engine::Signal::Signal() {
    this->Name = "Undefined";
}

Engine::Signal::~Signal() {
    delete this->Data;
}

template <typename Type>
void Engine::Signal::SetValue(Type value, int LuaType) {
    this->Data = new Type(value);
    this->LuaType = LuaType;
}

template <typename Type>
Type Engine::Signal::GetValue() {
    Type *ret = static_cast<Type*>(this->Data);
    return *ret;
}
