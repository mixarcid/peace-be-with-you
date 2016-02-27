#pragma once

#include "RenderableComp.hpp"

NAMESPACE {

  struct RenderableShapeData {
    
    Vec3f pos;
    Vec4f color;

    RenderableShapeData(Vec3f _pos, Vec4f _color);
    
  };

  struct RenderableShape : RenderableReg {

    Vec4f color;

    RenderableShape(Vec4f _color);
    virtual void render(RenderContext c);

    static RenderableShape CUBE;
    static RenderableShape SPHERE;
    
    static void init();
    
    };
  
}
