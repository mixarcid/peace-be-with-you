#pragma once

#include "Containers.hpp"
#include "VertexObjects.hpp"
#include "BoundingObject.hpp"
#include "Shader.hpp"

NAMESPACE {

  struct ShaderVar;

  struct RenderContext {
    f32 dt;
    RenderContext(f32 delta_time);
  };

  struct Renderable {

    ShaderFlags shader_flags;

    Renderable(ShaderFlags _shader_flags);

    virtual BoundingObject getTightBoundingObject();
    virtual BoundingObject getLooseBoundingObject();
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
