#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#define ENGINE_LUA_INTEGER 6

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