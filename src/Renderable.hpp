#pragma once

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

    ShaderFlags shader_flags;

    Renderable(ShaderFlags _shader_flags);

    //Yeah, weird naming convention...
    //bounding box used by graphics:
    virtual BoundingObject getBoundingObject() = 0;
    //bounding box used by physics for initial collision test:
    virtual BoundingObject getPhysicalBoundingObject() = 0;
    //bounding box used by physics for final collision test:
    virtual BoundingObject getPrimaryBoundingObject() = 0;
    virtual void render(RenderContext c) = 0;
    virtual ~Renderable() {}
    
  };
  
  struct RenderableReg : Renderable {

    VBO vbo;
    VAO vao;
    EBO ebo;

    using Renderable::Renderable;
    void init();
    void updateBuffers();
    virtual void render(RenderContext c);
    
  };
  
}
