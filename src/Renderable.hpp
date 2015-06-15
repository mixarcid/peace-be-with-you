#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;

  template <typename T>
    struct Renderable {

    VBO vbo;
    VAO vao;
    EBO ebo;
    Array<T> data;
    Array<GLuint> elements;
    
    void init(Array<ShaderVar> shader_vars, bool is_dynamic) {
      vao.init();
      vao.use();
      vbo.init();
      ebo.init();
      vbo.bindArray(data, is_dynamic);
      ebo.bindArray(elements, is_dynamic);
      vao.registerVars(shader_vars);
    }
    virtual void render() {
      vao.use();
      ebo.draw(GL_TRIANGLES);
    }
    virtual ~Renderable() {}
    
  };
  
}
