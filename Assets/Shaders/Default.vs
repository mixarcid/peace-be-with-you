out vec4 color;
out vec2 tex;
smooth out vec3 normal;
void main() {
  color = inColor;
  tex = inTexCoord;
  normal = normalize(transpose(inverse(mat3(uniModel))) * inNormal);
  gl_Position = uniProj * uniView *
    uniModel * vec4(inPosition, 1.0);
}
