#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;
  struct BoundingObject;
  
  struct Renderable {

    VBO vbo;
    VAO vao;
    EBO ebo;
    
    void init();
    virtual BoundingObject* getBoundingObject() = 0;
    virtual BoundingObject* getPhysicalBoundingObject() = 0;
    virtual void render();
    virtual ~Renderable() {}
    
  };
  
}
