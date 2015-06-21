#pragma once

#include "PhysicalObjects.hpp"
#include "Containers.hpp"
#include "System.hpp"

NAMESPACE {

  struct Physics {

    Array<StaticObject*> static_objects;
    Array<DynamicObject*> dynamic_objects;

    Vec3f gravity;

    Physics();
    void update(float dt);
    unsigned int addStaticObject(StaticObject* obj);
    //void removeStaticObject(unsigned int index);
    unsigned int addDynamicObject(DynamicObject* obj);
    //void removeStaticObject(unsigned int index);
    
  };

}
