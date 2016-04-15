#pragma once

#include "GameObject.hpp"
#include "Assets.hpp"
#include "Texture.hpp"
#include "TexQuad.hpp"

NAMESPACE {

  struct Sun : DynamicObject {

    static Asset<Texture> texture;

    Sun() : DynamicObject(Vec3f(0,0,0)) {
      $rttiConstruct("Sun");
      TexQuad* q = new TexQuad(*texture.get(), Vec2f(500, 500));
      addComponent(q);
      GameObject::loose_object.set(&q->b_sphere);
      GameObject::tight_object.set(&q->b_sphere);
    }
    void init(Pointer<DynamicObject>& player);
    ~Sun();
    
  };
  $registerRttiStruct();
  
}
