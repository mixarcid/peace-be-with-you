#pragma once

#include "GameObject.hpp"
#include "RenderableComp.hpp"

NAMESPACE {

  struct WaterRenderable : RenderableReg {

    WaterRenderable(Vec2f halves);
    
  };

  struct Water : StaticObject {
    
    Water(Vec3f pos, Vec2f halves) : StaticObject(pos) {
      $rttiConstruct("Water");
      addComponent(new WaterRenderable(halves));
      BoundingAABB aabb(pos, Vec3f(halves, 10));
      GameObject::loose_object.set(&aabb);
      GameObject::tight_object.set(&aabb);
    }

    ~Water() {
      delete getComponent<RenderableComp>();
    }
    
  };
  $registerRttiStruct();
  

}
