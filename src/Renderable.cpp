#include "Renderable.hpp"


NAMESPACE {

  void RenderableReg::init() {
    vao.init();
    vao.use();
    vbo.init();
    ebo.init();
  }
  void RenderableReg::render() {
    vao.use();
    ebo.draw(GL_TRIANGLES);
  }

}
