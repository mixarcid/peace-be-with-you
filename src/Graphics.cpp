#include "Graphics.hpp"

NAMESPACE {
  
  Graphics::Graphics(String shader_name) : shade(shader_name) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    shade.use();
  }

  void Graphics::render(GLFWwindow* win, f32 dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shade.use();
    Graphics3d::render(win, dt);
    Graphics2d::render(win);
  }
}
