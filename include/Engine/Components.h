#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

namespace Engine {
    class GameComponent {
    public:
        bool Alive;
        int TextureID = -1;

        SDL_Rect Rect, SrcRect;

        GameComponent() {
            this->Alive = true;
        }

        ~GameComponent() {
            
        }

        virtual void Init() {}
        virtual void Update(float delta_time) {}
        virtual void Render() {}
    };

    class LuaComponent : public GameComponent {
    public:
        // Script
        lua_State *LuaState;
        int LuaRow;

        // Component
        LuaComponent(const char* path);

        LuaComponent();

        ~LuaComponent();

        bool LoadScript(const char *path);

        void ScriptInit();

        void ScriptUpdate(float delta_time);
    };

    class Group {
    public:
        std::vector<GameComponent*> Components;

        Group();

        void Update(float delta_time);
        void Render();

        bool CheckCollision(SDL_Rect*);
    };

    class Scene {
    public:
        virtual ~Scene(){};

        bool Pause;
        int MouseX, MouseY;

        std::unordered_map<std::string, Group> Groups;

        virtual void Init() {}
        virtual void InputDown(SDL_Keycode) {}
        virtual void InputUp(SDL_Keycode) {}
        virtual void MouseMotion(SDL_MouseButtonEvent) {}
        virtual void MouseDown(SDL_MouseButtonEvent) {}
        virtual void MouseUp(SDL_MouseButtonEvent) {}
        virtual void Update(float delta) {}
        virtual void Render() {}
    };

    class AnimationManager {
    public:
        AnimationManager(SDL_Rect* sheet);

        void Update(float delta_time);

        void Play(const char* name);

        void Stop();

        void CreateAnimation(const char* name, std::vector<int_fast8_t> frames);

        void SetAnimationSpeed(float seconds);

        std::string Finished;
        bool Loop;

        std::string Name;

    //private:
        bool p = false; // Playing Some Animation

        Uint32 las_tim; // Last Time

        float at = 1.0f; // Animation Time in ms

        int_fast8_t f = 0;  // Animation Frame, Index Animation
        int_fast8_t fr = 0; // Reference Animation Frame
        int_fast8_t jmp = 16; // Size of unique sprite to jump
        std::vector<int_fast8_t> anim;

        std::unordered_map<const char*, std::vector<int_fast8_t>> anims;

        SDL_Rect* s; // Sheet
    };

    class TileMap : public GameComponent {
    public:
        TileMap();
        TileMap(int width, int height);

        void Render() override;
    };

    class Signal {
        Engine::Any Data;

    public:
        Signal();

        std::string Name;

        void SetValue(Engine::Any value);

        template <typename type> 
        Engine::Any GetValue();

    };

    extern std::unordered_map<std::string, Group> CurrentGroups;
    extern std::vector<Signal> Signals;
}

#endif