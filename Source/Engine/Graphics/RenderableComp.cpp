
#include "RenderableComp.hpp"


NAMESPACE {

  void RenderableReg::init() {
    Shader::setFlags(RenderableComp::shader_flags);
    vao.init();
    vao.use();
    ebo.init();
    vbo.init();
  }
  
  void RenderableReg::render(RenderContext c) {
    vao.use();
    ebo.draw(c.instances);
  }

}
