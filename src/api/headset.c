#include "api/lovr.h"
#include "headset/headset.h"

map_int_t ControllerAxes;
map_int_t ControllerButtons;
map_int_t ControllerHands;
map_int_t HeadsetEyes;
map_int_t HeadsetOrigins;
map_int_t HeadsetTypes;

typedef struct {
  lua_State* L;
  int ref;
} HeadsetRenderData;

static HeadsetRenderData headsetRenderData;

static void renderHelper(HeadsetEye eye, void* userdata) {
  HeadsetRenderData* renderData = userdata;
  lua_State* L = renderData->L;
  lua_rawgeti(L, LUA_REGISTRYINDEX, renderData->ref);
  luax_pushenum(L, &HeadsetEyes, eye);
  lua_call(L, 1, 0);
}

int l_lovrHeadsetInit(lua_State* L) {
  lua_newtable(L);
  luaL_register(L, NULL, lovrHeadset);
  luax_registertype(L, "Controller", lovrController);

  map_init(&ControllerAxes);
  map_set(&ControllerAxes, "trigger", CONTROLLER_AXIS_TRIGGER);
  map_set(&ControllerAxes, "grip", CONTROLLER_AXIS_GRIP);
  map_set(&ControllerAxes, "touchx", CONTROLLER_AXIS_TOUCHPAD_X);
  map_set(&ControllerAxes, "touchy", CONTROLLER_AXIS_TOUCHPAD_Y);

  map_init(&ControllerButtons);
  map_set(&ControllerButtons, "unknown", CONTROLLER_BUTTON_UNKNOWN);
  map_set(&ControllerButtons, "system", CONTROLLER_BUTTON_SYSTEM);
  map_set(&ControllerButtons, "menu", CONTROLLER_BUTTON_MENU);
  map_set(&ControllerButtons, "trigger", CONTROLLER_BUTTON_TRIGGER);
  map_set(&ControllerButtons, "grip", CONTROLLER_BUTTON_GRIP);
  map_set(&ControllerButtons, "touchpad", CONTROLLER_BUTTON_TOUCHPAD);
  map_set(&ControllerButtons, "a", CONTROLLER_BUTTON_A);
  map_set(&ControllerButtons, "b", CONTROLLER_BUTTON_B);
  map_set(&ControllerButtons, "x", CONTROLLER_BUTTON_X);
  map_set(&ControllerButtons, "y", CONTROLLER_BUTTON_Y);

  map_init(&ControllerHands);
  map_set(&ControllerHands, "unknown", HAND_UNKNOWN);
  map_set(&ControllerHands, "left", HAND_LEFT);
  map_set(&ControllerHands, "right", HAND_RIGHT);

  map_init(&HeadsetEyes);
  map_set(&HeadsetEyes, "left", EYE_LEFT);
  map_set(&HeadsetEyes, "right", EYE_RIGHT);

  map_init(&HeadsetOrigins);
  map_set(&HeadsetOrigins, "head", ORIGIN_HEAD);
  map_set(&HeadsetOrigins, "floor", ORIGIN_FLOOR);

  map_init(&HeadsetTypes);
  map_set(&HeadsetTypes, "unknown", HEADSET_UNKNOWN);
  map_set(&HeadsetTypes, "vive", HEADSET_VIVE);
  map_set(&HeadsetTypes, "rift", HEADSET_RIFT);

  lovrHeadsetInit();

  headsetRenderData.ref = LUA_NOREF;

  return 1;
}

int l_lovrHeadsetIsPresent(lua_State* L) {
  lua_pushboolean(L, lovrHeadsetIsPresent());
  return 1;
}

int l_lovrHeadsetGetType(lua_State* L) {
  luax_pushenum(L, &HeadsetTypes, lovrHeadsetGetType());
  return 1;
}

int l_lovrHeadsetGetOriginType(lua_State* L) {
  luax_pushenum(L, &HeadsetOrigins, lovrHeadsetGetOriginType());
  return 1;
}

int l_lovrHeadsetIsMirrored(lua_State* L) {
  lua_pushboolean(L, lovrHeadsetIsMirrored());
  return 1;
}

int l_lovrHeadsetSetMirrored(lua_State* L) {
  int mirror = lua_toboolean(L, 1);
  lovrHeadsetSetMirrored(mirror);
  return 0;
}

int l_lovrHeadsetGetDisplayWidth(lua_State* L) {
  int width;
  lovrHeadsetGetDisplayDimensions(&width, NULL);
  lua_pushnumber(L, width);
  return 1;
}

int l_lovrHeadsetGetDisplayHeight(lua_State* L) {
  int height;
  lovrHeadsetGetDisplayDimensions(NULL, &height);
  lua_pushnumber(L, height);
  return 1;
}

int l_lovrHeadsetGetDisplayDimensions(lua_State* L) {
  int width, height;
  lovrHeadsetGetDisplayDimensions(&width, &height);
  lua_pushnumber(L, width);
  lua_pushnumber(L, height);
  return 2;
}

int l_lovrHeadsetGetClipDistance(lua_State* L) {
  float near, far;
  lovrHeadsetGetClipDistance(&near, &far);
  lua_pushnumber(L, near);
  lua_pushnumber(L, far);
  return 2;
}

int l_lovrHeadsetSetClipDistance(lua_State* L) {
  float near = luaL_checknumber(L, 1);
  float far = luaL_checknumber(L, 2);
  lovrHeadsetSetClipDistance(near, far);
  return 0;
}

int l_lovrHeadsetGetBoundsWidth(lua_State* L) {
  lua_pushnumber(L, lovrHeadsetGetBoundsWidth());
  return 1;
}

int l_lovrHeadsetGetBoundsDepth(lua_State* L) {
  lua_pushnumber(L, lovrHeadsetGetBoundsDepth());
  return 1;
}

int l_lovrHeadsetGetBoundsDimensions(lua_State* L) {
  lua_pushnumber(L, lovrHeadsetGetBoundsWidth());
  lua_pushnumber(L, lovrHeadsetGetBoundsDepth());
  return 2;
}

int l_lovrHeadsetGetBoundsGeometry(lua_State* L) {
  float geometry[12];
  lovrHeadsetGetBoundsGeometry(geometry);
  lua_newtable(L);
  for (int i = 0; i < 4; i++) {
    lua_newtable(L);
    for (int j = 0; j < 3; j++) {
      lua_pushnumber(L, geometry[3 * i + j]);
      lua_rawseti(L, -2, j + 1);
    }
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

int l_lovrHeadsetGetPosition(lua_State* L) {
  float x, y, z;
  lovrHeadsetGetPosition(&x, &y, &z);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pushnumber(L, z);
  return 3;
}

int l_lovrHeadsetGetEyePosition(lua_State* L) {
  float x, y, z;
  HeadsetEye eye = *(HeadsetEye*) luax_optenum(L, 1, "left", &HeadsetEyes, "eye");
  lovrHeadsetGetEyePosition(eye, &x, &y, &z);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pushnumber(L, z);
  return 3;
}

int l_lovrHeadsetGetOrientation(lua_State* L) {
  float angle, x, y, z;
  lovrHeadsetGetOrientation(&angle, &x, &y, &z);
  lua_pushnumber(L, angle);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pushnumber(L, z);
  return 4;
}

int l_lovrHeadsetGetVelocity(lua_State* L) {
  float x, y, z;
  lovrHeadsetGetVelocity(&x, &y, &z);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pushnumber(L, z);
  return 3;
}

int l_lovrHeadsetGetAngularVelocity(lua_State* L) {
  float x, y, z;
  lovrHeadsetGetAngularVelocity(&x, &y, &z);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  lua_pushnumber(L, z);
  return 3;
}

int l_lovrHeadsetGetControllers(lua_State* L) {
  vec_controller_t* controllers = lovrHeadsetGetControllers();
  if (!controllers) {
    lua_newtable(L);
    return 1;
  }

  lua_newtable(L);
  Controller* controller; int i;
  vec_foreach(controllers, controller, i) {
    luax_pushtype(L, Controller, controller);
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

int l_lovrHeadsetGetControllerCount(lua_State* L) {
  vec_controller_t* controllers = lovrHeadsetGetControllers();
  if (controllers) {
    lua_pushnumber(L, controllers->length);
  } else {
    lua_pushnumber(L, 0);
  }
  return 1;
}

int l_lovrHeadsetRenderTo(lua_State* L) {
  lua_settop(L, 1);
  luaL_checktype(L, 1, LUA_TFUNCTION);

  if (headsetRenderData.ref != LUA_NOREF) {
    luaL_unref(L, LUA_REGISTRYINDEX, headsetRenderData.ref);
  }

  headsetRenderData.ref = luaL_ref(L, LUA_REGISTRYINDEX);
  headsetRenderData.L = L;
  lovrHeadsetRenderTo(renderHelper, &headsetRenderData);
  return 0;
}

const luaL_Reg lovrHeadset[] = {
  { "isPresent", l_lovrHeadsetIsPresent },
  { "getType", l_lovrHeadsetGetType },
  { "getOriginType", l_lovrHeadsetGetOriginType },
  { "isMirrored", l_lovrHeadsetIsMirrored },
  { "setMirrored", l_lovrHeadsetSetMirrored },
  { "getDisplayWidth", l_lovrHeadsetGetDisplayWidth },
  { "getDisplayHeight", l_lovrHeadsetGetDisplayHeight },
  { "getDisplayDimensions", l_lovrHeadsetGetDisplayDimensions },
  { "getClipDistance", l_lovrHeadsetGetClipDistance },
  { "setClipDistance", l_lovrHeadsetSetClipDistance },
  { "getBoundsWidth", l_lovrHeadsetGetBoundsWidth },
  { "getBoundsDepth", l_lovrHeadsetGetBoundsDepth },
  { "getBoundsDimensions", l_lovrHeadsetGetBoundsDimensions },
  { "getBoundsGeometry", l_lovrHeadsetGetBoundsGeometry },
  { "getPosition", l_lovrHeadsetGetPosition },
  { "getEyePosition", l_lovrHeadsetGetEyePosition },
  { "getOrientation", l_lovrHeadsetGetOrientation },
  { "getVelocity", l_lovrHeadsetGetVelocity },
  { "getAngularVelocity", l_lovrHeadsetGetAngularVelocity },
  { "getControllers", l_lovrHeadsetGetControllers },
  { "getControllerCount", l_lovrHeadsetGetControllerCount },
  { "renderTo", l_lovrHeadsetRenderTo },
  { NULL, NULL }
};
