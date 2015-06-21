#include "Physics.hpp"

NAMESPACE {

  Physics::Physics() : gravity(0,-4,0) {}

  void Physics::update(float dt) {
    for (DynamicObject* obj : dynamic_objects) {
      obj->updatePhysics(dt);
    }
  }

  unsigned int Physics::addStaticObject(StaticObject* obj) {
    static_objects.push_back(obj);
    return static_objects.size() - 1;
  }
  
  unsigned int Physics::addDynamicObject(DynamicObject* obj) {
    obj->addForce(gravity*obj->mass);
    dynamic_objects.push_back(obj);
    return dynamic_objects.size() - 1;
  }

}
