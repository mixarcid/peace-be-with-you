#pragma once

#include "PhysicsComp.hpp"

NAMESPACE {

  struct Engine;
  
  struct Physics {

    Engine* engine;
    DynamicPhysicsComp::MovingObjectArray moving_objects;
    Vec3f gravity;

    Physics(Engine* _engine);
    void update();
    
  };

}
