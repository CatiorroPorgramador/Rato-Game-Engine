extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int hello(lua_State* L) {
    lua_pushstring(L, "Hello!");
    return 1;
}

static const struct luaL_Reg functions[] = {
    {"Hello", hello},
    {NULL, NULL}
};

int luaopen_mylibrary(lua_State* L) {
    luaL_newlib(L, functions);
    return 1;
}
}