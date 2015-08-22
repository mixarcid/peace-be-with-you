#ifdef SHADER_USE_COLOR
flat in vec4 color;
#else
in vec2 tex;
#endif
in vec3 normal;
in vec4 test;

uniform vec3 light_dir = normalize(vec3(-1,1,-1));
//uniform float light_intensity = 2;
uniform float ambient_intensity = 0.1f;
uniform vec4 test_color = vec4(1.0, 1.0f, 1.0f, 1.0f);

void main() {
  float diffuse_intensity = max(0.0, dot(normalize(normal), -light_dir));
  float intensity = diffuse_intensity;
  if (intensity > 0.75f) {
    intensity = 1.0f;
  } else if (intensity > 0.5f) {
    intensity = 0.75f;
  } else if (intensity > 0.25f) {
    intensity = 0.5f;
  } else {
    intensity = 0.25f;
  }
  outColor = vec4(test_color.rgb*intensity, 1.0f)*texture(uniTexture, tex);
}
