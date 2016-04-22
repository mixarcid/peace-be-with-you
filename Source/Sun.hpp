#pragma once

#include "GameObject.hpp"
#include "Assets.hpp"
#include "Texture.hpp"
#include "TexQuad.hpp"

NAMESPACE {

  struct Sun : DynamicObject {

    Sun(f32 dist) : DynamicObject(Vec3f(0,0,0)) {
      player_dist = dist;
      $rttiConstruct("Sun");
      TexQuad* q = new TexQuad(*texture.get(),
			       Vec2f(0.5*player_dist,
				     0.5*player_dist));
      addComponent(q);
      GameObject::loose_object.set(&q->b_sphere);
      GameObject::tight_object.set(&q->b_sphere);
    }
    void init();
    ~Sun();

    static f32 player_dist;
    static Asset<Texture> texture;
    
  };
  $registerRttiStruct();
  
}
