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

        virtual void Update() {}
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

        void Update();
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

        void Update();

        void Play(const char* name);

        void Stop();

        void CreateAnimation(const char* name, std::vector<int_fast8_t> frames);

        void SetAnimationSpeed(int_fast16_t speed);

        std::string Finished;
        bool Loop = true;

        std::string Name;

    private:
        bool p = false; // Playing Some Animation
        int_fast16_t i = 0; // Animation Index
        int_fast16_t as = 30; // Animation Speed

        int_fast8_t f = 0;  // Animation Frame, Index Animation
        int_fast8_t fr = 0; // Reference Animatoin Frame
        int_fast8_t jmp = 16; // Size of unique sprite to jump
        std::vector<int_fast8_t> anim;

        std::unordered_map<const char*, std::vector<int_fast8_t>> anims;

        SDL_Rect* s; // Sheet
    };

    extern std::unordered_map<std::string, Group> CurrentGroups;
}

#endif