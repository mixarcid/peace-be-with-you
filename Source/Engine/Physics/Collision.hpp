#pragma once

#include "Messageable.hpp"
#include "PhysicsComp.hpp"
#include "GameObject.hpp"

NAMESPACE {

  struct CollisionMessage : Message {

    //the object the messagee collided with
    ComponentPair<PhysicsComp> object;

    CollisionMessage(ComponentPair<PhysicsComp> _object)
      : object(_object) {
      $rttiConstruct("CollisionMessage");
    }
  };
  $registerRttiStruct();
  
  bool resolveCollision(ComponentPair<PhysicsComp> a,
			ComponentPair<PhysicsComp> b,
			bool second_arg_is_static);
  
}
