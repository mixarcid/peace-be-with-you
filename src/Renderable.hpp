#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;
  
  struct Renderable {

    VBO vbo;
    VAO vao;
    EBO ebo;
    
    void init();
    virtual void render();
    virtual ~Renderable() {}
    
  };
  
}
