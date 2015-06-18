in vec4 color;
in vec2 tex;
in vec3 normal;

uniform vec3 light_dir = normalize(vec3(-1,-1,-1));
//uniform float light_intensity = 2;
uniform float ambient_intensity = 0.1f;
uniform vec4 test_color = vec4(1.0, 1.0f, 1.0f, 1.0f);

void main() {
  float diffuse_intensity = max(0.0, dot(normalize(normal), -light_dir));
  float intensity = diffuse_intensity;
  outColor = vec4(test_color.rgb*intensity, 1.0f)*texture(uniTexture, tex);
}
