#include "math/math.h"
#include "lib/map/map.h"
#include "lib/glfw.h"
#include "util.h"

#pragma once

#define LOVR_SHADER_POSITION 0
#define LOVR_SHADER_NORMAL 1
#define LOVR_SHADER_TEX_COORD 2
#define LOVR_MAX_UNIFORM_LENGTH 256

typedef enum {
  SHADER_DEFAULT,
  SHADER_SKYBOX,
  SHADER_FONT,
  SHADER_FULLSCREEN
} DefaultShader;

typedef struct {
  GLchar name[LOVR_MAX_UNIFORM_LENGTH];
  int index;
  int location;
  GLenum type;
  int count;
} Uniform;

typedef map_t(Uniform) map_uniform_t;

typedef struct {
  Ref ref;
  int id;
  map_uniform_t uniforms;
  float model[16];
  float view[16];
  float projection[16];
  Color color;
} Shader;

Shader* lovrShaderCreate(const char* vertexSource, const char* fragmentSource);
Shader* lovrShaderCreateDefault(DefaultShader type);
void lovrShaderDestroy(const Ref* ref);
void lovrShaderBind(Shader* shader, mat4 model, mat4 view, mat4 projection, Color color, int force);
int lovrShaderGetAttributeId(Shader* shader, const char* name);
int lovrShaderGetUniformId(Shader* shader, const char* name);
int lovrShaderGetUniformType(Shader* shader, const char* name, GLenum* type, int* count);
void lovrShaderSendInt(Shader* shader, int id, int value);
void lovrShaderSendFloat(Shader* shader, int id, float value);
void lovrShaderSendFloatVec2(Shader* shader, int id, int count, float* vector);
void lovrShaderSendFloatVec3(Shader* shader, int id, int count,float* vector);
void lovrShaderSendFloatVec4(Shader* shader, int id, int count, float* vector);
void lovrShaderSendFloatMat2(Shader* shader, int id, float* matrix);
void lovrShaderSendFloatMat3(Shader* shader, int id, float* matrix);
void lovrShaderSendFloatMat4(Shader* shader, int id, float* matrix);
