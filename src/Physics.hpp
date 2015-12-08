#pragma once

#include "PhysicalObject.hpp"
#include "Containers.hpp"
#include "Octree.hpp"
#include "System.hpp"

NAMESPACE {

  struct Physics {

    Octree static_objects;
    Array<PhysicalObject*> dynamic_objects;

    Vec3f gravity;

    Physics(Vec3f center, Vec3f halves);
    void update(f32 dt);
    void addStaticObject(StaticObject* obj);
    u32 addDynamicObject(PhysicalObject* obj);
    //void removeStaticObject(u32 index);
    
  };

}
