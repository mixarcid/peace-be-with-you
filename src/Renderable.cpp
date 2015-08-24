#include "Renderable.hpp"


NAMESPACE {

  RenderContext::RenderContext(f32 delta_time)
    : dt(delta_time) {}

  Renderable::Renderable(ShaderFlags _shader_flags)
    : shader_flags(_shader_flags) {}

  void RenderableReg::init() {
    Shader::setFlags(Renderable::shader_flags);
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
