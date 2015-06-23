#pragma once

#include "PhysicalObject.hpp"
#include "Containers.hpp"
#include "System.hpp"

NAMESPACE {

  struct Physics {

    Array<StaticObject*> static_objects;
    Array<PhysicalObject*> dynamic_objects;

    Vec3f gravity;

    Physics();
    void update(f32 dt);
    u32 addStaticObject(StaticObject* obj);
    //void removeStaticObject(u32 index);
    u32 addDynamicObject(PhysicalObject* obj);
    //void removeStaticObject(u32 index);
    
  };

}
