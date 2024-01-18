#ifndef ENGINE_LIBRARY_H
#define ENGINE_LIBRARY_H

namespace Engine {
    namespace Library {
        int __LoadDefaultLibraries(lua_State*);

        int __IsInputDown(lua_State*);

        int __HasCollisionInGroup(lua_State*);

        int __EmitSignal(lua_State*);

        int __GetSignal(lua_State*);

        int __LoadTextureID(lua_State*);
        
        static const struct luaL_Reg Engine [] = {
            {"LoadDefaultLibraries", __LoadDefaultLibraries},
            {"IsInputDown", __IsInputDown},
            {"HasCollisionInGroup", __HasCollisionInGroup},
            {"EmitSignal", __EmitSignal},
            {"GetSignal", __GetSignal},
            {"LoadTextureID", __LoadTextureID},
            {NULL, NULL}
        };

        // ...
        void Register(lua_State *L);
    }
}

#endif