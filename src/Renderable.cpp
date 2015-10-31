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
    ebo.init();
    vbo.init();
  }
  
  void RenderableReg::render(RenderContext c) {
    vao.use();
#ifdef PEACE_GL_USE_POINTS
    ebo.draw(GL_POINTS);
#else
    ebo.draw(GL_TRIANGLES);
#endif
  }

}
