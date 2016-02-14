#include "ScreenQuad.hpp"

NAMESPACE {

  Array<Vec2f> ScreenQuad::data = {
    Vec2f(-1.0f, -1.0f),
    Vec2f(1.0f, -1.0f),
    Vec2f(-1.0f, 1.0f),
    Vec2f(-1.0f, 1.0f),
    Vec2f(1.0f, -1.0f),
    Vec2f(1.0f,  1.0f)
  };

  void ScreenQuad::init(ShaderVar var) {
    vao.init();
    vao.use();
    vbo.init();
    vbo.bindArray(data, false);
    vao.registerVars({var});
  }
  
  void ScreenQuad::render() {
    vao.use();
    vbo.draw(GL_TRIANGLES);
  }
}
