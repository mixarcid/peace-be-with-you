#pragma once

#include "GameObject.hpp"
#include "RenderableComp.hpp"

NAMESPACE {

  struct WaterRenderable : RenderableReg {

    WaterRenderable(Vec2f halves);
    /*virtual void render(RenderContext c) {
      Log::message("!");
      }*/
    
  };

  struct Water : DynamicObject {
    
    Water(Vec3f pos, Vec2f halves) : DynamicObject(pos) {
      $rttiConstruct("Water");
      init(pos, halves);
    }

    void init(Vec3f pos, Vec2f halves);
    
    ~Water() {
      delete getComponent<RenderableComp>();
    }

    virtual void message(Message* msg);
    
  };
  $registerRttiStruct();
  

}
