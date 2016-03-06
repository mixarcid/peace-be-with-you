#pragma once

#include "Messageable.hpp"
#include "PhysicsComp.hpp"
#include "GameObject.hpp"

NAMESPACE {

  struct CollisionMessage : Message {

    //the object the messagee collided with
    Pointer<GameObject> object;

    CollisionMessage(Pointer<GameObject> _object)
      : object(_object) {
      $rttiConstruct("CollisionMessage");
    }
  };
  $registerRttiStruct();

  bool resolveCollision(DynamicComponentPair<DynamicPhysicsComp> a,
			DynamicComponentPair<DynamicPhysicsComp> b);
  bool resolveCollision(DynamicComponentPair<DynamicPhysicsComp> a,
			StaticComponentPair<StaticPhysicsComp> b);
  
}
