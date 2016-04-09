out vec4 outColor;
out vec3 outNormal;
out vec2 outOffset;
//out vec3 outDepth;

#ifdef SHADER_USE_COLOR
flat in vec4 color;
#endif

#ifdef SHADER_USE_TEXTURE
in vec2 tex;
#endif

#ifdef SHADER_USE_NORMAL
in vec3 normal;
#endif

#ifdef SHADER_TERRAIN
in vec4 biome_data;
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
      intensity = 0.2f;
    }
    light_color += uniDirLights[i].color*intensity;
  }
  outColor *= vec4(light_color, 1);
#endif
#endif

#ifdef SHADER_USE_COLOR
  outColor *= color;
#endif
  
#if defined(SHADER_USE_TEXTURE) && !defined(SHADER_TERRAIN)
  outColor *= texture(uniTexture, tex);
#endif

#ifdef SHADER_TERRAIN
  
  vec2 tex_coords[4];
  tex_coords[0] = tex;
  tex_coords[1] = tex - vec2(0.25,0);
  tex_coords[2] = tex - vec2(0,0.25);
  tex_coords[3] = tex - vec2(0.25,0.25);

  vec3 color = vec3(0,0,0);
  for (uint n = 0u; n < 4u; ++n) {
    color += biome_data[n]*(texture(uniTexture, tex_coords[n]).xyz);
  }
  outColor.xyz *= color;
#endif
  
#ifdef SHADER_2D
  outColor *= uniColor;
#endif

#ifdef SHADER_USE_TEXTURE
  outOffset = texture(uniPaint, vec2(0.5) + 0.5*norm.xy).xy;
#else
  outOffset = vec2(0.5,0.5);
#endif

  //outDepth = vec3(gl_FragCoord);

}
