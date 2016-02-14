in vec2 screenCoord;
out vec2 texCoord;

void main() {
  texCoord = vec2(0.5,0.5)+(screenCoord/2);
  gl_Position = vec4(screenCoord, 0, 1);
}
