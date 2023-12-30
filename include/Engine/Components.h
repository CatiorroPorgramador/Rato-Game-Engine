#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

namespace Engine {
    class GameComponent {
    public:
        SDL_Rect Rect;

        GameComponent() {
            
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
}

#endif