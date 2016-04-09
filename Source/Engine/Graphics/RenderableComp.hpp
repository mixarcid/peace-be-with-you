#pragma once

#include "GLObject.hpp"
#include "Shader.hpp"
#include "Component.hpp"

NAMESPACE {

  struct RenderContext {
    u32 instances;
    f32 dt;
    Vec3f dist;
  };

  struct RenderableComp : Component {

    ShaderFlags shader_flags;

    virtual void render(RenderContext c) = 0;
    
    RenderableComp(ShaderFlags _shader_flags)
      : shader_flags(_shader_flags) {
      $rttiConstruct("RenderableComp");
    }
      
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
