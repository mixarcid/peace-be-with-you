#pragma once

#include "PhysicsComp.hpp"

NAMESPACE {

  struct Engine;
  
  struct Physics {

    Engine* engine;
    Vec3f gravity;

    Physics(Engine* _engine);
    void update();
    
  };

}
