const uint MAX_DIR_LIGHTS = 1;
struct DirLight {
  vec3 dir;
  //float pad0[1];
  vec3 color;
};

layout(std140) uniform _uniDirLights {
  DirLight uniDirLights[MAX_DIR_LIGHTS];
};
layout(std140) uniform _uniAmbient {
  float uniAmbient;
};

#ifndef SHADER_USE_COLOR
uniform sampler2D uniTexture;
#endif
out vec4 outColor;
