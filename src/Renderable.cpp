#include "Renderable.hpp"


NAMESPACE {

  RenderContext::RenderContext(f32 delta_time)
    : dt(delta_time) {}

  void RenderableReg::init() {
    vao.init();
    vao.use();
    vbo.init();
    ebo.init();
  }
  void RenderableReg::render(RenderContext c) {
    vao.use();
    ebo.draw(GL_TRIANGLES);
  }

}
