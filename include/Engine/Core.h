#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

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
    class Any {
    public:
        Any() : data(nullptr), type(nullptr) {}

        template <typename T>
        Any(const T& value) : data(new T(value)), type(&typeid(T)) {}

        ~Any();

        template <typename type>
        type Cast() const;

    private:
        void* data;
        const std::type_info* type;
    };

    void Init(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer);

    void End();
}

#endif