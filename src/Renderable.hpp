#pragma once

#include "Standard.hpp"
#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;
  struct BoundingObject;

  struct RenderContext {
    f32 dt;
    RenderContext(f32 delta_time);
  };

  struct Renderable {
    
    virtual BoundingObject getBoundingObject() = 0;
    virtual BoundingObject getPhysicalBoundingObject() = 0;
    virtual BoundingObject getPrimaryBoundingObject() = 0;
    virtual void render(RenderContext c) = 0;
    virtual ~Renderable() {}
    
  };
  
  struct RenderableReg : Renderable {

    VBO vbo;
    VAO vao;
    EBO ebo;
    
    void init();
    virtual void render(RenderContext c);
    
  };
  
}
