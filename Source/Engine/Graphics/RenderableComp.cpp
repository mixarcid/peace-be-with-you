#include "RenderableComp.hpp"


NAMESPACE {

  RenderContext::RenderContext(f32 delta_time)
    : dt(delta_time) {}

  void RenderableReg::init(VBO _vbo,
			   VAO _vao,
			   EBO _ebo) {
    Shader::setFlags(RenderableComp::shader_flags);
    if (_vao.flags & PEACE_GL_UNINITIALIZED) {
      vao.init();
    } else {
      vao = _vao;
    }
    vao.use();
    if (_ebo.flags & PEACE_GL_UNINITIALIZED) {
      ebo.init();
    } else {
      ebo = _ebo;
    }
    if (_vbo.flags & PEACE_GL_UNINITIALIZED) {
      vbo.init();
    } else {
      vbo = _vbo;
    }
  }
  
  void RenderableReg::render(RenderContext c) {
    vao.use();
    ebo.draw();
  }

}
