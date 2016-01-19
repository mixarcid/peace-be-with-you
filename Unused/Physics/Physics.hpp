#pragma once

#include "PhysicsComponent.hpp"

NAMESPACE {
  
  struct Physics {

    Vec3f gravity;

    Physics(Engine* engine);
    void update();
    
  };

}
