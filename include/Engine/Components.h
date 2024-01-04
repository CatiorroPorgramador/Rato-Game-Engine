#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

namespace Engine {
    class GameComponent {
    public:
        bool Alive;

        Uint16 TextureID;

        SDL_Rect Rect;

        GameComponent() {
            this->Alive = true;
        }

        ~GameComponent() {
            
        }
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

    extern std::unordered_map<std::string, Group> CurrentGroups;
}

#endif