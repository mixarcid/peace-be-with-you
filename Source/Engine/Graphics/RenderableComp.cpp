#include "RenderableComp.hpp"


NAMESPACE {

  RenderContext::RenderContext(f32 delta_time)
    : dt(delta_time) {}

  void RenderableReg::init() {
    Shader::setFlags(RenderableComp::shader_flags);
    vao.init();
    vao.use();
    ebo.init();
    vbo.init();
  }
  
  void RenderableReg::render(RenderContext c) {
    vao.use();
    ebo.draw();
  }

}
