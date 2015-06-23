#include "Physics.hpp"
#include "Collision.hpp"

NAMESPACE {

  Physics::Physics() : gravity(0,-9.8,0) {}

  void Physics::update(f32 dt) {
    for (PhysicalObject* a : dynamic_objects) {
      a->force = gravity*a->mass_data.mass;
      a->update(dt);
    }
    
    for (PhysicalObject* a : dynamic_objects) {
      for (StaticObject* b : static_objects) {
	resolveCollision(a,b);
      }
      for (PhysicalObject* b : dynamic_objects) {
	resolveCollision(a,b);
      }
    }
  }

  u32 Physics::addStaticObject(StaticObject* obj) {
    static_objects.push_back(obj);
    return static_objects.size() - 1;
  }
  
  u32 Physics::addDynamicObject(PhysicalObject* obj) {
    //obj->applyForce(gravity*obj->mass_data.mass);
    dynamic_objects.push_back(obj);
    return dynamic_objects.size() - 1;
  }

}
