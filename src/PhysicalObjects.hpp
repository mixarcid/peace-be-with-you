#pragma once

#include "Node.hpp"
#include "Containers.hpp"

NAMESPACE {

  typedef Node StaticObject;

  struct DynamicObject : Node {

    float mass;

    Vec3f veloc;
    //basically forces, but we've already divided the mass
    Array<Vec3f> accels;

    DynamicObject(float m, Vec3f xi, Vec3f vi = Vec3f(0,0,0));
    void updatePhysics(float dt);
    unsigned int addForce(Vec3f force);
    void removeForce(unsigned int index);
    
  };
  
}
