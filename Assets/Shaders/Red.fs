#ifdef SHADER_USE_COLOR
flat in vec4 color;
#else
in vec2 tex;
#endif
in vec3 normal;
in vec4 test;

void main() {
#ifndef SHADER_2D
  vec3 norm = normalize(normal);
  vec3 light_color = vec3(uniAmbient);
  for (uint i=0; i<MAX_DIR_LIGHTS; ++i) {
    float intensity = max(0.0,
			  dot(norm,
			      -uniDirLights[i].dir));
    if (intensity > 0.75f) {
      intensity = 1.0f;
    } else if (intensity > 0.5f) {
      intensity = 0.75f;
    } else if (intensity > 0.25f) {
      intensity = 0.5f;
    } else {
      intensity = 0.25f;
    }
    light_color += uniDirLights[i].color*intensity;
  }
#endif
#ifdef SHADER_2D
#ifdef SHADER_USE_COLOR
  outColor = color;
#else
  outColor = uniColor*texture(uniTexture, tex);
#endif
#else
#ifdef SHADER_USE_COLOR
  outColor = vec4(light_color,1)*color;
#else
  outColor = vec4(1,0,0,1);//vec4(light_color,1)*texture(uniTexture, tex);
#endif
#endif
}
