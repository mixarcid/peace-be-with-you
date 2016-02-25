out vec4 outColor;
out vec3 outNormal;

#ifdef SHADER_USE_COLOR
flat in vec4 color;
#endif

#ifdef SHADER_USE_TEXTURE
in vec2 tex;
#endif

#ifdef SHADER_USE_NORMAL
in vec3 normal;
#endif

in vec3 pos;

void main() {

  outColor = vec4(1,1,1,1);
  
#ifdef SHADER_USE_NORMAL
  
  outNormal = normal;
  vec3 norm = normalize(normal);
  
#ifdef SHADER_3D
  vec3 light_color = vec3(uniAmbient);
  for (uint i=0; i<MAX_DIR_LIGHTS; ++i) {
    float intensity = max(0.0,
			  dot(norm,
			      -uniDirLights[i].dir));
    if (intensity > 0.75f) {
      intensity = 1.0f;
    } else if (intensity > 0.5f) {
      intensity = 0.7f;
    } else if (intensity > 0.25f) {
      intensity = 0.4f;
    } else {
      intensity = 0.0f;
    }
    light_color += uniDirLights[i].color*intensity;
  }
  outColor *= vec4(light_color, 1);
#endif
#endif
  
#ifdef SHADER_USE_COLOR
  outColor *= color;
#endif
#ifdef SHADER_USE_TEXTURE
  outColor *= texture(uniTexture, tex);
#endif

#ifdef SHADER_2D
  outColor *= uniColor;
#endif
  
}
