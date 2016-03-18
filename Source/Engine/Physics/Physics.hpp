#pragma once

#include "PhysicsComp.hpp"

NAMESPACE {

  struct Engine;
  
  struct Physics {

    Engine* engine;
    DynamicPhysicsComp::MovingObjectArray moving_objects;
    Vec3f gravity;

    Physics(Engine* _engine);
    //both functions add the total number of checks and collisions
    //to the input values
    //updateVsDynamic also finalizes everything, so it should
    //be called last
    void updateVsDynamic(u32* num_checks, u32* num_collisions);
    void updateVsStatic(u32* num_checks, u32* num_collisions);
    
  };

}
