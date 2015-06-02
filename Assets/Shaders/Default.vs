out vec4 color;
void main() {
  color = inColor;
  gl_Position = vec4(inPosition, 1.0);
}
