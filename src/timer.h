#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int lovrTimerStep(lua_State* L);

extern const luaL_Reg lovrTimer[];
int lovrPushTimer(lua_State* L);