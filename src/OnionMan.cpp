#include "OnionMan.h"
#include "Shader.h"

NAMESPACE {

  OnionMan::OnionMan() {
    data = {
      {Vec3f(-0.5, 0.5, 0), Color4f(1,0,0)},
      {Vec3f(0.5, 0.5, 0), Color4f(1,0,0)},
      {Vec3f(-0.5, -0.5, 0), Color4f(1,0,0)},
      {Vec3f(0.5, 0.5, 0), Color4f(1,0,0)},
      {Vec3f(-0.5, -0.5, 0), Color4f(1,0,0)},
      {Vec3f(0.5, -0.5, 0), Color4f(0,1,0)}
    };
    vao.use();
    vbo.bindArray(data, false);
    vao.registerVars({Shader::POSITION, Shader::COLOR});
  }

  void OnionMan::render() {
    vao.use();
    vbo.draw(GL_TRIANGLES);
  }
}
