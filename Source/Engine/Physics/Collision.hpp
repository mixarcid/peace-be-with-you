#pragma once

#include "Message.hpp"
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

  bool resolveCollision(Pointer<DynamicObject>& a_obj,
			Pointer<DynamicPhysicsComp>& a_comp,
			Pointer<DynamicObject>& b_obj,
			Pointer<DynamicPhysicsComp>& b_comp);
  bool resolveCollision(Pointer<DynamicObject>& a_obj,
			Pointer<DynamicPhysicsComp>& a_comp,
			Pointer<StaticObject>& b_obj,
			Pointer<StaticPhysicsComp>& b_comp);
  
}
