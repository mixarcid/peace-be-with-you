#pragma once

#include "PhysicalObject.hpp"
#include "HandledArray.hpp"
#include "Octree.hpp"
#include "System.hpp"

NAMESPACE {

  struct Physics {

    Octree static_objects;
    HandledArray<PhysicalObject*> dynamic_objects;

    Vec3f gravity;

    Physics(Vec3f center, Vec3f halves);
    void update(f32 dt);
    void addStaticObject(StaticObject* obj);
    ArrayHandle addDynamicObject(PhysicalObject* obj);
    void removeDynamicObject(ArrayHandle h);
    ArrayHandle addObject(PhysicalObject* obj);
    void removeObject(ArrayHandle h);
    
  };

}
