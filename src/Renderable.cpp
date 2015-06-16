#include "Renderable.hpp"


NAMESPACE {

  void Renderable::init() {
    vao.init();
    vao.use();
    vbo.init();
    ebo.init();
  }
  void Renderable::render() {
    vao.use();
    ebo.draw(GL_TRIANGLES);
  }

}
