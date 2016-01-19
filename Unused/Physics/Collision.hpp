#pragma once

#include "PhysicalObject.hpp"

NAMESPACE {

  struct CollisionMessage : Message {

    //the object the messagee collided with
    PhysicalObject* object;

    CollisionMessage(PhysicalObject* _object)
      : object(_object) {
      $rttiConstruct("CollisionMessage");
    }
  };
  $registerRttiStruct();
  
  void resolveCollision(PhysicalObject* a, PhysicalObject* b);
  
}
