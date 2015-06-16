#pragma once

#include "Node.hpp"

NAMESPACE {

  typedef Node StaticObject;

  struct DynamicObject : Node {

    float mass;

    Vec3f veloc;
    Vec3f accel;
    
  }
  
}
