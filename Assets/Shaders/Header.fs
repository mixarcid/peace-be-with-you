const uint MAX_DIR_LIGHTS = 1;
struct DirLight {
  vec3 dir;
  //float pad0[1];
  vec3 color;
};

#ifdef SHADER_3D
layout(std140) uniform _uniDirLights {
  DirLight uniDirLights[MAX_DIR_LIGHTS];
};

layout(std140) uniform _uniAmbient {
  float uniAmbient;
};
#endif

#ifdef SHADER_USE_TEXTURE
uniform sampler2D uniTexture;
#endif

#ifdef SHADER_2D
layout(std140) uniform _uniColor {
  vec4 uniColor;
};
#endif
