#pragma once

#include "GLObject.hpp"
#include "Shader.hpp"
#include "Component.hpp"

NAMESPACE {

  struct RenderContext {
    f32 dt;
    f32 dist;
  };

  struct RenderableComp : Component {

    ShaderFlags shader_flags;

    RenderableComp(ShaderFlags _shader_flags)
      : shader_flags(_shader_flags) {
      $rttiConstruct("RenderableComp");
    }
    virtual void render(RenderContext c) = 0;
    
  };
  $registerRttiStruct();
  
  struct RenderableReg : RenderableComp {

    VBO vbo;
    VAO vao;
    EBO ebo;

    using RenderableComp::RenderableComp;
    void init();
    virtual void render(RenderContext c);
    
  };
  
}
