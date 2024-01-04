#ifndef ENGINE_LIBRARY_H
#define ENGINE_LIBRARY_H

namespace Engine {
    namespace Library {
        int __LoadDefaultLibraries(lua_State *L);

        int __IsInputDown(lua_State *L);

        int __HasCollisionInGroup(lua_State *L);

        int __EmitSignalToComponent(lua_State *L);

        int __LoadTextureID(lua_State *L);
        
        static const struct luaL_Reg Engine [] = {
            {"LoadDefaultLibraries", __LoadDefaultLibraries},
            {"IsInputDown", __IsInputDown},
            {"HasCollisionInGroup", __HasCollisionInGroup},
            {"EmitSignalToComponent", __EmitSignalToComponent},
            {"LoadTextureID", __LoadTextureID},
            {NULL, NULL}
        };

        // ...
        void Register(lua_State *L);
    }
}

#endif